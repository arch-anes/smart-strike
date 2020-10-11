FROM node:slim
COPY ./server/dist dist
COPY ./docker-entrypoint.sh docker-entrypoint.sh
CMD [ "./docker-entrypoint.sh" ]
