# Observations
1. `index` only  
- Send the index file
- If the target is a folder, respond with `301 Moved Permanently` with `Location` header
- default error 404 page
2. `return 301 URL`
- **Applied no matter what**
- Respond with `301 Moved Permanently` with `Location` header
- The client send a new request!
- Can be `Location` dependent.
3. `error_page 404 /404.html`
- allows to define an 404 error page
4. `autoindex on`
- works only with `target`s ending with `/`
- else error 404
5. **symbolic links are followed!**
6. Method not allowed `403 Forbidden`

# Index.html
## Request
```
GET / HTTP/1.1
Host: localhost
User-Agent: curl/7.68.0
Accept: */*
```
## Response

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
