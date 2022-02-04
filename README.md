# webserv
A lightweight HTTP server implemented in C++.

---

## To-DO-List

 - [ ] Webserv
	- [x] Config file parsing
	- [x] Connection management
	- [ ] Request processing (Common Core)
	- [ ] Basic request handling (static content)
	- [ ] CGI request handling
	- [ ] Basic response sending
	- [ ] CGI response sending
 - [ ] Create a default error page
 - [ ] Create a POC static website
 - [ ] Create a POC CGI content

## Change Requests

### Config File
 - [ ] Set up a default config file path if none is provided
 - [ ] Add a HOST to a listening port
### Connection Management
<!-- To avoid bugs or dull checks: 
     - deque should never be empty
     - last item should always be ready to hold the next incomming request
-->
 - [ ] In the CLientSocket's constructor create an empty `Request` ans set its state as `NONE`
 - [ ] When moving a `Request` from `DOWNLOADED` to `PROCESSING` create an empty `Request` and set its state as `NONE`
### Request Common Core
### Request Static Site
### Request CGI
### Response Static Site
### Response CGI

---

## Ressources

### RFC

- [2616: Hypertext Transfer Protocol](https://www.rfc-editor.org/rfc/pdfrfc/rfc2616.txt.pdf)
- [3875: The Common Gateway Interface (CGI)](https://www.rfc-editor.org/rfc/pdfrfc/rfc3875.txt.pdf)
- [7230: Message Syntax and Routing](https://www.rfc-editor.org/rfc/pdfrfc/rfc7230.txt.pdf)
- [7231: Semantics and Content](https://www.rfc-editor.org/rfc/pdfrfc/rfc7231.txt.pdf)
- [7232: Conditional Requests](https://www.rfc-editor.org/rfc/pdfrfc/rfc7232.txt.pdf)
- [7233: Range Requests](https://www.rfc-editor.org/rfc/pdfrfc/rfc7233.txt.pdf)
- [7234: Caching](https://www.rfc-editor.org/rfc/pdfrfc/rfc7234.txt.pdf)
- [7235: Authentication](https://www.rfc-editor.org/rfc/pdfrfc/rfc7235.txt.pdf)

### Other

- [C++ Web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
- [How the web works: HTTP and CGI explained](https://www.garshol.priv.no/download/text/http-tut.html)
- [Everything you need to know to Build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [Building a simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
- [Understanding and Implementing FastCGI Proxying in Nginx](https://www.digitalocean.com/community/tutorials/understanding-and-implementing-fastcgi-proxying-in-nginx)
- [tderwedu's random facts](https://github.com/tderwedu/42cursus/blob/main/12_Inception/RandomFacts.md)
- [Documentation Webserv (in French)](https://www.notion.so/etelcode/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46)
- [Understanding the Nginx Configuration File Structure and Configuration Contexts](https://www.digitalocean.com/community/tutorials/understanding-the-nginx-configuration-file-structure-and-configuration-contexts)

## License

This repository is released under the [Unlicense](https://github.com/maxdesalle/webserv/blob/main/LICENSE).
