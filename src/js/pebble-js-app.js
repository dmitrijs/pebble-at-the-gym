var MessageQueue = function () {
    var RETRY_MAX = 5;
    var queue = [];
    var sending = false;
    var timer = null;
    return {reset: reset, sendAppMessage: sendAppMessage, size: size};
    function reset() {
        queue = [];
        sending = false
    }

    function sendAppMessage(message, ack, nack) {
        if (!isValidMessage(message)) {
            return false
        }
        queue.push({message: message, ack: ack || null, nack: nack || null, attempts: 0});
        setTimeout(function () {
            sendNextMessage()
        }, 1);
        return true
    }

    function size() {
        return queue.length
    }

    function isValidMessage(message) {
        if (message !== Object(message)) {
            return false
        }
        var keys = Object.keys(message);
        if (!keys.length) {
            return false
        }
        for (var k = 0; k < keys.length; k += 1) {
            var validKey = /^[0-9a-zA-Z-_]*$/.test(keys[k]);
            if (!validKey) {
                return false
            }
            var value = message[keys[k]];
            if (!validValue(value)) {
                return false
            }
        }
        return true;
        function validValue(value) {
            switch (typeof value) {
                case"string":
                    return true;
                case"number":
                    return true;
                case"object":
                    if (toString.call(value) == "[object Array]") {
                        return true
                    }
            }
            return false
        }
    }

    function sendNextMessage() {
        if (sending) {
            return
        }
        var message = queue.shift();
        if (!message) {
            return
        }
        message.attempts += 1;
        sending = true;
        Pebble.sendAppMessage(message.message, ack, nack);
        timer = setTimeout(function () {
            timeout()
        }, 1e3);
        function ack() {
            clearTimeout(timer);
            setTimeout(function () {
                sending = false;
                sendNextMessage()
            }, 200);
            if (message.ack) {
                message.ack.apply(null, arguments)
            }
        }

        function nack() {
            clearTimeout(timer);
            if (message.attempts < RETRY_MAX) {
                queue.unshift(message);
                setTimeout(function () {
                    sending = false;
                    sendNextMessage()
                }, 200 * message.attempts)
            } else {
                if (message.nack) {
                    message.nack.apply(null, arguments)
                }
            }
        }

        function timeout() {
            setTimeout(function () {
                sending = false;
                sendNextMessage()
            }, 1e3);
            if (message.ack) {
                message.ack.apply(null, arguments)
            }
        }
    }
}();
/*

 function fetchNotes(max_content_length) {
 var response;
 var req = new XMLHttpRequest();
 req.open('GET', "http://notes.wbjnh.com/service.php?action=get_outside&pebble=1", true);
 req.onload = function (e) {
 if (req.readyState == 4) {
 if (req.status == 200) {
 console.log('web response: ' + req.responseText);
 response = JSON.parse(req.responseText);
 var outside = response.outside;
 // response == { "outside" : [ "guid" : <guid>, "content" : <content> ] }

 var notes_data = '';
 notes_data += outside.length + ';';

 for (var ind in outside) {
 if (!outside.hasOwnProperty(ind)) continue;
 var item = outside[ind];

 var notes_data_item = item['guid'] + ';' + (item['content'].length + 2) + ';- ' + item['content'] + ';';
 if (notes_data.length + notes_data_item.length > max_content_length) {
 // nothing fits anymore
 console.log("this did not fit: " + notes_data_item);
 break;
 }

 notes_data += notes_data_item;
 }

 console.log("notes_data = " + notes_data);

 MessageQueue.sendAppMessage({
 "notes_data": notes_data
 });

 } else {
 console.log("Request returned error code " + req.status.toString());

 MessageQueue.sendAppMessage({
 "error": "Web code " + req.status.toString()
 });
 }
 }
 };
 req.send(null);
 }
 */

function saveWorkoutData(group, operation, data) {

    MessageQueue.sendAppMessage({
        "group": group,
        "operation": operation,
        "data": 'ok'
    });

    /*var response;
     var req = new XMLHttpRequest();
     req.open('GET', "http://notes.wbjnh.com/service.php?pebble=1&action=save&POST[guid]=" + guid + "&POST[is_done]=1", true);
     req.onload = function (e) {
     if (req.readyState == 4) {
     if (req.status == 200) {

     // do not fetch notes immediately. item will be marked as done and will disappear on next update
     //fetchNotes();

     //MessageQueue.sendAppMessage({
     //    "error": "No error. Item was marked as done."
     //});
     }
     }
     };
     req.send(null);*/
}

Pebble.addEventListener("ready", function (e) {
    console.log("JSApp is ready");

    //fetchNotes(2000);

    MessageQueue.sendAppMessage({
        "ready": 1
    });
});

Pebble.addEventListener("appmessage", function (e) {
    console.log("Message from pebble: " + JSON.stringify(e.payload));

    //if (e.payload.fetch) {
    //    fetchNotes(e.payload.max_len - 50);
    //}
    //if (e.payload.done) {
    //    markNoteDone(e.payload.done, e.payload.max_len - 50);
    //}

    if (e.payload.group) {
        saveWorkoutData(e.payload.group, e.payload.operation, e.payload.data);
    }
});
