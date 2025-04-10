run: build
	docker run -d --name ft_ssl -v $(shell pwd)/../:/app ft_ssl

build:
	docker build -t ft_ssl .

exec:
	docker exec -it ft_ssl bash

stop:
	docker stop ft_ssl

fclean: stop
	docker rm ft_ssl
	docker image rm ft_ssl

re: run build exec stop fclean
