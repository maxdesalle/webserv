FROM debian:buster

#=========================== 1. Intall Dependencies ============================

# Update Software Packages
RUN apt-get -qq update && apt-get -yqq upgrade

# To install NGINX
RUN apt-get -yqq install nginx

#======================== 2. Install & Configure Nginx =========================

# Copy NGINX server block definition and create a symbolic link
# to "enable" our website
COPY srcs/website.conf /etc/nginx/sites-available
RUN ln -s /etc/nginx/sites-available/website.conf /etc/nginx/sites-enabled

# Root directory of our website
RUN mkdir -p /var/www/website
WORKDIR /var/www/website

COPY ./website/ .

# RUN cd .. ; chown -R www-data:www-data * && chmod -R 755 /var/www/* ; cd website

#=========================== 3. Launching Container ===========================

# To allow a graceful NGINX shutdown
STOPSIGNAL SIGQUIT

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
