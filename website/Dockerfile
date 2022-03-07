#Installing and updating Debian Buster
FROM debian:buster
RUN apt-get update
RUN apt-get upgrade -y

# Update Software Packages
RUN apt-get -qq update && apt-get -yqq upgrade
# To install NGINX
RUN apt-get -yqq install nginx

#======================== 2. Install & Configure Nginx =========================

# Copy NGINX server block definition and create a symbolic link to "enable" our website
COPY website.conf /etc/nginx/sites-available
RUN ln -s /etc/nginx/sites-available/website.conf /etc/nginx/sites-enabled

#Deleting original self-signed SSL certificate and key (normally only for HTTPS)
RUN rm /etc/nginx/sites-enabled/default
RUN rm /etc/nginx/sites-available/default

#Copying the "conf.conf" file created to configure Nginx
COPY srcs/conf.conf /etc/nginx/sites-available
#A symbolic link has to be created to enable the Nginx config
RUN ln -s /etc/nginx/sites-available/website.conf /etc/nginx/sites-enabled

# Root directory of our website
COPY ./website/ /var/www/website
WORKDIR /var/www/

RUN cd .. ; chown -R www-data:www-data * && chmod -R 755 /var/www/* ; cd website

#Starting up Nginx
COPY ./config.sh ./
CMD bash config.sh

#=========================== 3. Launching Container ===========================

# To allow a graceful NGINX shutdown
# STOPSIGNAL SIGQUIT

# EXPOSE 80

# CMD ["nginx", "-g", "daemon off;"]
