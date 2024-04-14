import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import sqlite3
from datetime import datetime

DATABASE_NAME = 'User.db'

def get_activity_data():
    conn = sqlite3.connect(DATABASE_NAME)
    cursor = conn.cursor()
    cursor.execute("SELECT timestamp FROM user_activity")
    data = cursor.fetchall()
    conn.close()
    timestamps = []
    for record in data:
        try:
            clean_timestamp = record[0].strip().rstrip(')') 
            timestamp = datetime.strptime(clean_timestamp, '%Y-%m-%d %H:%M:%S')
            timestamps.append(timestamp)
        except ValueError as e:
            print(f"Error parsing '{record[0]}': {e}")
    return timestamps


fig, ax = plt.subplots()

def update_graph(frame):
    timestamps = get_activity_data()
    ax.clear()
    ax.plot_date(timestamps, range(len(timestamps)), '-') 
    ax.set(title='User Activity Over Time', xlabel='Time', ylabel='Activity Count')


ani = FuncAnimation(fig, update_graph, interval=1000)

plt.show()
