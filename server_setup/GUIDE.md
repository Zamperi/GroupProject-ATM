# Server setup

## Installing Ansible on Ubuntu

`$ sudo apt update`

`$ sudo apt upgrade`

`$ sudo apt install ansible`

## Setting up the database

Copy the `server_setup/ansible` directory somewhere on the server. On the server, run:

`cd ansible`

Ensure that `group_vars/all.yaml` has the correct values for the server!

`ansible-playbook deploy_database.yaml`

On the development machine, open an ssh tunnel:

`ssh -L 3306:localhost:3306 user@[SERVER]`

## Setting up the REST API

Copy the `backend` directory somewhere in the server

`cd ansible`

Ensure that `group_vars/all.yaml` has the correct values for the server!

`ansible-playbook deploy_api.yaml`

On the client, open an ssh tunnel:

`ssh -L 3000:localhost:3000 ubuntu@172.20.241.89`

## Notes

I tried to open a port on the server so that the client would not need to ssh tunnel and could instead use the IP address of the server. Checking the route between the machines with `traceroute`, though, revealed there were three machines between the server and client, any of which may not allow me to use port 3000.