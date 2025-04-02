from flask import Flask, request, jsonify
import mysql.connector

app = Flask(__name__)

db_config = {
    'host': 'localhost',
    'user': 'root',
    'password': '',
    'database': 'game_db'
}

@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')

    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()

    # 유저 찾기
    cursor.execute("SELECT * FROM users WHERE username = %s", (username,))
    user = cursor.fetchone()

    if user:
        # 유저 존재 → 비밀번호 확인
        cursor.execute("SELECT * FROM users WHERE username = %s AND password = %s", (username, password))
        user_match = cursor.fetchone()
        if user_match:
            return jsonify({'success': True, 'message': '로그인 성공'})
        else:
            return jsonify({'success': False, 'message': '비밀번호가 틀렸습니다'})
    else:
        # 유저 없음 → 회원가입 진행
        cursor.execute("INSERT INTO users (username, password) VALUES (%s, %s)", (username, password))
        conn.commit()
        return jsonify({'success': True, 'message': '회원가입 후 로그인 완료'})

    cursor.close()
    conn.close()