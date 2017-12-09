var temperature_in = "js8837euf_temperature_in";
var temperature_out = "js8837euf_temperature_out";
var humidity = "js8837euf_humidity";

$(document).ready(function() {
  var socket = io.connect('http://' + document.domain + ':' + location.port);
  // $('#publish').click(function(event) {
  //   var topic = $('#topic').val()
  //   var message = $('#message').val()
  //   var data = '{"topic": "' + topic + '", "message": "' + message + '"}';
  //   socket.emit('publish', data=data);
  // });
  $('#subscribe').click(function(event) {
    socket.emit('subscribe');
    $('#subscribe').hide();
    $('#unsubscribe').show();
  });
  $('#unsubscribe').click(function(event) {
    // socket.emit('unsubscribe_all');
    $('#subscribe').show();
    $('#unsubscribe').hide();
  });
  socket.on('mqtt_message', function(data) {
    console.log(data);
    var topic = data['topic'];
    var measure = data['payload'];
    var source;
    switch(topic) {
        case temperature_in:
          source = "Temperature indoor: ";
          break;
        case temperature_out:
          source = "Temperature outdoor: ";
          break;
        case humidity:
          source = "Humidity indoor: ";
          break;
        default:
          source = "";
    }
    var $textarea = $('#subscribe_messages');
    $textarea.val($textarea.val() + source + measure + '\n');
  })
});