# Return 301 + Auto Index
## Request
```
GET / HTTP/1.1
Host: localhost
User-Agent: curl/7.68.0
Accept: */*
```
## Response
```
HTTP/1.1 301 Moved Permanently
Server: nginx/1.14.2
Date: Wed, 16 Feb 2022 16:23:26 GMT
Content-Type: text/html
Content-Length: 185
Connection: keep-alive
Location: https://localhost/

<html>
<head><title>301 Moved Permanently</title></head>
<body bgcolor="white">
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.14.2</center>
</body>
</html>
```

# Auto Index
## Request
```
GET / HTTP/1.1
Host: localhost
User-Agent: curl/7.68.0
Accept: */*
```
## Response
```
HTTP/1.1 200 OK
Server: nginx/1.14.2
Date: Wed, 16 Feb 2022 16:29:40 GMT
Content-Type: text/html
Transfer-Encoding: chunked
Connection: keep-alive

<html>
<head><title>Index of /</title></head>
<body bgcolor="white">
<h1>Index of /</h1><hr><pre><a href="../">../</a>
<a href="phpmyadmin/">phpmyadmin/</a>                                        16-Feb-2022 16:19                   -
<a href="wordpress/">wordpress/</a>                                         16-Feb-2022 16:19                   -
<a href="start.sh">start.sh</a>                                           07-Nov-2021 14:38                  82
</pre><hr></body>
</html>
```

# 
