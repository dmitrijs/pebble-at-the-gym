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

function saveWorkout(whole_group, group, operation) {
    var whole_data = whole_group.join('\n');

    var response;
    var req = new XMLHttpRequest();
    req.open('GET', "http://pebble.wbjnh.com/at_the_gym/?pebble=1&action=save_workout&data=" + escape(whole_data), false);
    req.onload = function (e) {
        console.log("e: " + req.readyState + " " + req.status + " group: " + group + " op: " + operation);

        if (req.readyState == 4) {
            if (req.status == 200) {

                MessageQueue.sendAppMessage({
                    "group": group,
                    "operation": operation,
                    "data": 'saved'
                });
            }
        }
    };
    req.send(null);
}

var groups = {0: [], 1: [], 2: []};
function isGroupComplete(grp) {
    console.log("grp count: " + grp.length);
    return grp.length == 12;
}

function saveWorkoutData(group, operation, data) {

    groups[group].push(operation + '=' + data);

    if (!isGroupComplete(groups[group])) {
        MessageQueue.sendAppMessage({
            "group": group,
            "operation": operation,
            "data": 'need_more'
        });
    } else {
        saveWorkout(groups[group], group, operation);
        groups[group] = [];
    }
}

Pebble.addEventListener("ready", function (e) {
    console.log("JSApp is ready");

    MessageQueue.sendAppMessage({
        "ready": 1
    });
});

Pebble.addEventListener("appmessage", function (e) {
    console.log("Message from pebble: " + JSON.stringify(e.payload));

    if (e.payload.group) {
        saveWorkoutData(e.payload.group, e.payload.operation, e.payload.data);
    }
});
