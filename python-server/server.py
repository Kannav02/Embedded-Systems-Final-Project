import socket
import sqlite3
from datetime import datetime
import time
import matplotlib.pyplot as plt



DATABASE_NAME = 'User.db'

def validate_user(uid):
    conn = sqlite3.connect(DATABASE_NAME)
    cursor = conn.cursor()
    cursor.execute("SELECT first_name FROM user_info WHERE UID = ?", (uid,))
    result = cursor.fetchone()
    conn.close()
    return result[0] if result else None


def select_activity():
    conn=sqlite3.connect(DATABASE_NAME)
    cursor=conn.cursor()
    cursor.execute("SELECT timestamp FROM user_activity")
    result=cursor.fetchall()
    print(result)
    timestamps = [datetime.strptime(record[0], '%Y-%m-%d %H:%M:%S') for record in result]
    print(timestamps)
    conn.close()


def add_activity(uid, timestamp):
    conn = sqlite3.connect(DATABASE_NAME)
    cursor = conn.cursor()
    cursor.execute("INSERT INTO user_activity (UID, timestamp) VALUES (?, ?)", (uid, timestamp))
    conn.commit()
    conn.close()

def drop_records():
    conn=sqlite3.connect(DATABASE_NAME)
    cursor=conn.cursor()
    cursor.execute("DELETE FROM user_activity WHERE activity_id=1")
    conn.commit()
    conn.close()

def get_field_names(table_name):
    conn = sqlite3.connect(DATABASE_NAME)
    cursor = conn.cursor()
    cursor.execute(f"PRAGMA table_info({table_name});")
    columns_info = cursor.fetchall()

    conn.close()
    column_names = [info[1] for info in columns_info]
    print(column_names)

def start_server(host='0.0.0.0', port=8080):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)
    print(f"Listening on port {port}...")

    while True:
        client_socket, address = server_socket.accept()
        print(f"Connection from {address} has been established.")
        
        received_data = client_socket.recv(1024).decode('utf-8', errors='replace')
        print(f"Received data: {received_data}")

        try:
            uid, timestamp_str = received_data.strip("()").split(',')
            print(f"UID: {uid}, Timestamp: {timestamp_str}")
            
            user_name = validate_user(uid)
            if user_name:
                print(f"Valid user: {user_name}. Adding activity.")
                add_activity(uid, timestamp_str)
                
                client_socket.sendall(f'A, {user_name}'.encode('utf-8'))
            else:
                print(f"Invalid UID: {uid}.")
                client_socket.sendall(b'N')
        
        except Exception as e:
            print(f"Error processing received data: {e}")
            client_socket.sendall(b'E')  

        finally:
            client_socket.close()


if __name__ == "__main__":
    start_server()