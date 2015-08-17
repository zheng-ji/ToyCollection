server {
    listen     80;
    server_name share.91guanggao.com;
    #rewrite  "^/(.*)$"  http://aos.wall.youmi.net/$1 break;

    location / {
        return 404;
    }

    location /aos {
        rewrite /v(.+)/(.+) /v$1/$2 break;
        rewrite /v(.+) /v$1/s_goto break;
        proxy_pass http://aos.wall.youmi.net;
        include proxy_params_without_host;
    }

    location /ios {
        rewrite /v(.+)/(.+) /v$1/$2 break;
        rewrite /v(.+) /v$1/s_goto break;
        proxy_pass http://ios.wall.youmi.net;
        include proxy_params_without_host;
    }
}
