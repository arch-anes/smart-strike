FROM node:slim
COPY ./server/dist dist
CMD ["node", "dist/main"]
