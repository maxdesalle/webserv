build :
		docker build -t server .

run :
		docker run -it --rm -p 80:80 -p 8080:8080 webserv

cclean :
		docker rm $$(docker ps -qa)

iclean :
		docker rmi $$(docker images -q)

tclean :
		docker system prune
		
.phony:	build run rm re prune
