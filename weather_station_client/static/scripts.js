var temperature_in = "js8837euf_temperature_in";
var temperature_out = "js8837euf_temperature_out";
var humidity = "js8837euf_humidity";
var motion = "js8837euf_motion";

var temperature_in_value;
var temperature_out_value;
var humidity_value;
var motion_value;

$(document).ready(function() {
  var socket = io.connect('http://' + document.domain + ':' + location.port);
  $('#subscribe').click(function(event) {
    socket.emit('subscribe');
    $('#subscribe').hide();
    $('#unsubscribe').show();
  });
  $('#unsubscribe').click(function(event) {
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
          temperature_in_value = measure;
          // motion_value = "";
          break;
        case temperature_out:
          source = "Temperature outdoor: ";
          temperature_out_value = measure;
          // motion_value = "";
          break;
        case humidity:
          source = "Humidity indoor: ";
          humidity_value = measure;
          // motion_value = "";
          break;
        case motion:
          source = "Motion: ";
          motion_value = measure;
          break;
        default:
          source = "";
    }
    var $textarea = $('#subscribe_messages');
    $textarea.val($textarea.val() + source + measure + '\n');
    showWeather();
  })
});

function showWeather() {
    html = '<h3>Temperature indoor: '+temperature_in_value+'&deg;C</h3>';
    html += '<h3>Temperature outdoor: '+temperature_out_value+'&deg;C</h3>';
    html += '<h3>Humidity: '+humidity_value+'%</h3>';
    html += '<h3>Motion: '+motion_value+'</h3>';

    $("#weather").html(html);
    motion_value = "";
}
