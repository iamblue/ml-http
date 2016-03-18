# ml-http

## API

``` js

/* get api */

http({
  method: // string
  url:    //string
}, function(data) {
  // callback function
});

/* post api */

http({
  url: // string,
  method: // string,
  header: // string,
  contentType: // string,
}, function(data) {
  // callback function
});

```

## Example

``` js
/* get api */
  var config = {
    method: 'GET',
    url: 'http://apis.map.qq.com/ws/location/v1/ip?ip=61.135.17.68&key=6MABZ-VFKAF-DITJ6-JRPZN-OUOFJ-ULBWQ',
  };

  http(config, function(data) {
    print(data);
    var i = JSON.parse(data);
    print('status:' + i.status);
  });

/* post api */

  var config = {
    url: "http://",
    method: "POST",
    header: "deviceKey:123123\r\n";,
    contentType: "application/json",
  }

  http(config, function(data) {
    print('data: '+ data);
  });


```
