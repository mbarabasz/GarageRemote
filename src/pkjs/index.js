
function sendRequest() {
  var req = new XMLHttpRequest();
  req.open('POST', 'https://api.spark.io/v1/devices/53ff70065075535128591587/push/', true);
  req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        console.log('Call success');
      } else if (req.status === 201){
        console.log('Created');
        // Show the notification
        Pebble.showSimpleNotificationOnPebble('Success', 'Opened');
      } else {
        console.log('Response code: ' + req.status);
        Pebble.showSimpleNotificationOnPebble('Error', 'Response code: ' + req.status);
      }
    }else{
      console.log('Bad status');
      Pebble.showSimpleNotificationOnPebble('Error', 'Bad status');
    }
  };
  var payload = 'access_token=7316e2ec169363627a5ba6c4d97c2032c7ea8a03';
  console.log('payload sent to api: ' + payload);
  req.send(payload);
}

Pebble.addEventListener('ready', function (e) {
  console.log('connect!' + e.ready);
  console.log(e.type);
});

Pebble.addEventListener('appmessage', function (e) {
  console.log('message comming from C:' + e.type);
  sendRequest();
});