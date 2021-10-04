docker build . -t nginx_service
docker run -it -p 8080:80 nginx_service bash