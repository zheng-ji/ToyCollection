
1.minikube start  --image-mirror-country='cn' --image-repository='registry.cn-hangzhou.aliyuncs.com/google_containers'
2 kubectl create -f deployment.yaml 
kubectl delete -f deployment.yaml
3. kubectl expose deployment my-go-app --type=NodePort --name=go-app-svc --target-port=3000 
4.kubectl exec my-go-app-68f5b4c6cb-xkf7w -- ls /
5 通过运行kubectl exec -it multi-container-pod -c container-serving-dates -- bash连接到Nginx容器里。
6 kubectl scale --replicas=3 deployment my-go-app --record
扩容
