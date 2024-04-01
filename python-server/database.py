import sqlite3
from datetime import datetime

def create_tables():
    conn = sqlite3.connect('User.db')
    cursor = conn.cursor()

    # Create user_info table
    create_table_user_info_sql = '''CREATE TABLE IF NOT EXISTS user_info (
                                UID TEXT PRIMARY KEY,
                                first_name TEXT NOT NULL,
                                last_name TEXT NOT NULL)'''
    cursor.execute(create_table_user_info_sql)

    # Create user_activity table
    create_table_user_activity_sql = '''CREATE TABLE IF NOT EXISTS user_activity (
                                    activity_id INTEGER PRIMARY KEY AUTOINCREMENT,
                                    UID TEXT NOT NULL,
                                    timestamp DATETIME NOT NULL,
                                    FOREIGN KEY(UID) REFERENCES user_info(UID))'''
    cursor.execute(create_table_user_activity_sql)

    print("Tables created successfully")

    conn.commit()
    conn.close()

def insert_user(uid, first_name, last_name):
    conn = sqlite3.connect('User.db')
    cursor = conn.cursor()
    
    insert_sql = '''INSERT INTO user_info (UID, first_name, last_name) VALUES (?, ?, ?)'''
    try:
        cursor.execute(insert_sql, (uid, first_name, last_name))
        print(f"User {first_name} {last_name} added successfully.")
    except sqlite3.IntegrityError as e:
        print(f"Error adding user {first_name} {last_name}: {e}")
    
    conn.commit()
    conn.close()

if __name__ == "__main__":
    create_tables()

    # Insert some example users into the user_info table
    insert_user("83128CFB", "Oliver", "Moore")
