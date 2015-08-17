server
{
	listen  80;
	server_name  popot.r.91doujin.com;
	include deny_robot_favicon;
	access_log /home/ymserver/log/nginx/popot-r.access.log access_format ratio=0.001;

	location / {
		proxy_pass         http://gateway-servers;
		include proxy_params;
	}

	location /v1 {
		proxy_pass	http://jcx-servers;
		include proxy_params;
	}
}
