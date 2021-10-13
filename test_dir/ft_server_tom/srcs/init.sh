#!/bin/sh

#	SSL SETUP
mkdir /etc/nginx/ssl
openssl req -newkey rsa:4096 -x509 -sha256 -days 365 -nodes -out /etc/nginx/ssl/ftserver.crt -keyout /etc/nginx/ssl/ftserver.key -subj "/C=FR/ST=Paris/L=Paris/O=42 School/OU=tvachera/CN=ftserver"

#	NGINX SETUP
mkdir /var/www/ftserver
cp /root/nginx.conf /etc/nginx/sites-available/ftserver
ln -s /etc/nginx/sites-available/ftserver /etc/nginx/sites-enabled/ftserver
rm -f /etc/nginx/sites-enabled/default /etc/nginx/sites-available/default

#	WORDPRESS SETUP
wget -O /tmp/wordpress.tar.gz https://wordpress.org/latest.tar.gz
(cd /tmp && tar -xzf wordpress.tar.gz -C /var/www/ftserver/)
rm -f /tmp/wordpress.tar.gz
mv /root/wp-config.php /var/www/ftserver/wordpress/

#	PHP TEST SETUP
mkdir -p /var/www/ftserver/php
echo "<?php phpinfo(); ?>" > /var/www/ftserver/php/index.php

#	DATABASE SETUP
service mysql start
echo "CREATE DATABASE wordpress;" | mariadb -u root
echo "CREATE USER 'admin'@'localhost';" | mariadb -u root
echo "SET password FOR 'admin'@'localhost' = password('password');" | mariadb -u root
echo "GRANT ALL PRIVILEGES ON wordpress.* TO 'admin'@'localhost' IDENTIFIED BY 'password';" | mariadb -u root
echo "FLUSH PRIVILEGES;" | mariadb -u root
mysql wordpress -u root < /root/wordpress.sql

#	PHPMYADMIN SETUP
mkdir /var/www/ftserver/phpmyadmin
wget -O /tmp/phpmyadmin.tar.gz https://files.phpmyadmin.net/phpMyAdmin/4.9.0.1/phpMyAdmin-4.9.0.1-all-languages.tar.gz
(cd /tmp && tar -xzf phpmyadmin.tar.gz -C /var/www/ftserver/phpmyadmin/)
rm -f /tmp/phpmyadmin.tar.gz
mv /root/config.inc.php /var/www/ftserver/phpmyadmin

#	LAUNCHING
/etc/init.d/php7.3-fpm start
service nginx start
echo "Use \"autoindex on/off\" to activate or desactivate autoindex."
bash
