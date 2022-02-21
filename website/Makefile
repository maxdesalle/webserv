build:
		sudo docker build -t webserv .

run:
		sudo docker run -d --rm --name webserv -p 80:80 -p 8080:8080 webserv

rm:
	sudo docker rm -f webserv

re:		rm build run
		sudo docker ps

# To clean up temporary files
prune:
		sudo docker system prune
		
.phony:	build run rm re prune
