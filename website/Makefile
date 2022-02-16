# To bluid the 'ft_server' container
build:
		sudo docker build -t webserv .

# To run the 'ft_server' container
run:
		sudo docker run -d --rm --name webserv -p 80:80 webserv

# To clean up temporary files
prune:
		sudo docker system prune
		
.phony:	build run prune
