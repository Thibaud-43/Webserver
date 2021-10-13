FROM debian:buster

RUN	apt-get update -yq \
&& apt-get upgrade -yq \
&& apt-get install -yq wget mariadb-server php7.3 php7.3-fpm php7.3-mysql php-common php7.3-cli php7.3-common php7.3-json php7.3-opcache php7.3-readline nginx openssl

COPY srcs/nginx.conf ./root/
COPY srcs/init.sh ./bin/init_ftserver
COPY srcs/autoindex.sh ./bin/autoindex
COPY srcs/wordpress.sql /root/wordpress.sql
COPY srcs/config.inc.php /root/config.inc.php
COPY srcs/wp-config.php /root/

EXPOSE 80 443

CMD init_ftserver
