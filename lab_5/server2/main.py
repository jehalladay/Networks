import socket

def run():
    host = "127.0.0.1"
    port = 2345

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((host, port))
    s.listen()

    conn, addr = s.accept()

    print("Connected by", addr)

    while(True):
        data = conn.recv(1024)
        print("Received:", data)

        if not data:
            break
        print(data.decode())
        conn.send(data)


run()



