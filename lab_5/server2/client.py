import socket

# host = '192.168.1.6'
host = '192.168.1.16'
port = 5000
# port = 2345

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        print('Connecting')
        s.connect((host, port))
        print('sending')
        s.sendall(b'Hello, world2')
        print('recieving')
        data = s.recv(1024)
    # print('Received', repr(data))

    print('Received', data.decode('utf-8'))

main()