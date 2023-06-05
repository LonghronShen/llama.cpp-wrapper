FROM node:14 AS builder

WORKDIR /app

COPY . .

RUN npm install --save-dev && npm run build

FROM nginx:1.18 AS runtime

COPY --from=builder /app/dist /usr/share/nginx/html
COPY --from=builder /app/nginx/startup.sh /docker-entrypoint.d/
COPY --from=builder /app/nginx/nginx.conf.template /etc/nginx/conf.d/
COPY --from=longhronshens/smalte:1.01-bionic /app/smalte /usr/local/bin/

ENV BACKEND_HOST=server:5000

EXPOSE 80
