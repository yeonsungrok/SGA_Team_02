from flask import Flask, request, jsonify
import pymysql

app = Flask(__name__)

# ✅ MySQL 연결 설정
def get_conn():
    return pymysql.connect(
        host='localhost',
        user='root',
        password='',
        database='game_db',
        charset='utf8mb4',
        cursorclass=pymysql.cursors.DictCursor
    )


# ✅ 로그인 또는 회원가입
@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')

    conn = get_conn()
    cursor = conn.cursor()

    cursor.execute('SELECT * FROM users WHERE username = %s', (username,))
    user = cursor.fetchone()

    if user:
        if user['password'] == password:
            return jsonify({'success': True, 'message': '로그인 성공', 'username': username, 'first_in': False})
        else:
            return jsonify({'success': False, 'message': '비밀번호가 틀렸습니다.'}), 401
    else:
        cursor.execute('INSERT INTO users (username, password) VALUES (%s, %s)', (username, password))
        conn.commit()
        return jsonify({'success': True, 'message': '회원가입 완료', 'username': username, 'first_in': True, 'is_new': True})


# ✅ 스탯 저장
@app.route('/save_stats', methods=['POST'])
def save_stats():
    data = request.get_json()
    username = data.get('username')
    stats = data.get('stats', {})

    conn = get_conn()
    cursor = conn.cursor()

    cursor.execute('''
        REPLACE INTO stats (
            username, level, maxHp, curHp, maxMp, curMp,
            str_, dex, int_, exp, nextExp, bonusPoint,
            attackRadius, attackRange
        ) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)
    ''', (
        username,
        stats.get('level'),
        stats.get('maxHp'),
        stats.get('curHp'),
        stats.get('maxMp'),
        stats.get('curMp'),
        stats.get('str_'),
        stats.get('dex'),
        stats.get('int_'),
        stats.get('exp'),
        stats.get('nextExp'),
        stats.get('bonusPoint'),
        stats.get('attackRadius'),
        stats.get('attackRange')
    ))

    conn.commit()
    conn.close()
    return jsonify({'success': True})


# ✅ 스탯 불러오기
@app.route('/load_stats', methods=['GET'])
def load_stats():
    username = request.args.get('username')

    conn = get_conn()
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM stats WHERE username = %s', (username,))
    row = cursor.fetchone()
    conn.close()

    if not row:
        return jsonify({'success': False, 'message': 'No stats found'}), 404

    return jsonify(row)


# ✅ 인벤토리 저장
@app.route('/save_inventory', methods=['POST'])
def save_inventory():
    data = request.get_json()
    print("[DEBUG] save_inventory received:", data)

    username = data.get('username')
    inventory_list = data.get('inventory', [])
    equip_dict = data.get('equip', {})
    money = data.get('money', 0)

    conn = get_conn()
    cursor = conn.cursor()

    # 기존 아이템 삭제
    cursor.execute('DELETE FROM inventory WHERE username = %s', (username,))

    # 인벤토리 아이템 저장
    for item in inventory_list:
        if not item.get('code') or item.get('code') == 0:
            continue  # 유효하지 않은 아이템은 건너뜀

        cursor.execute('''
            INSERT INTO inventory (username, item_code, name, type, modTarget, description, price, value, equip)
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)
        ''', (
            username,
            item.get('code'),
            item.get('name'),
            item.get('type'),
            item.get('modTarget'),
            item.get('description'),
            item.get('price'),
            item.get('value'),
            0  # 인벤토리는 equip 슬롯 없음
        ))

    # 장착 아이템 저장 (code가 0이 아닌 것만)
    equip_slot_map = {
        "UpperArmor": 1,
        "LowerArmor": 2,
        "ShoulderArmor": 3,
        "Sword": 4,
        "Shield": 5
    }

    for slot, item in equip_dict.items():
        if not item.get('code') or item.get('code') == 0:
            continue  # 장착된 아이템이 아니면 저장하지 않음

        equip_slot = equip_slot_map.get(slot, 0)
        cursor.execute('''
            INSERT INTO inventory (username, item_code, name, type, modTarget, description, price, value, equip)
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)
        ''', (
            username,
            item.get('code'),
            item.get('name'),
            item.get('type'),
            item.get('modTarget'),
            item.get('description'),
            item.get('price'),
            item.get('value'),
            equip_slot
        ))

    conn.commit()
    conn.close()

    return jsonify({'status': 'success'})



@app.route('/load_inventory', methods=['GET'])
def load_inventory():
    username = request.args.get('username')
    conn = get_conn()
    cursor = conn.cursor(pymysql.cursors.DictCursor)

    cursor.execute('SELECT * FROM inventory WHERE username = %s', (username,))
    rows = cursor.fetchall()

    inventory = []
    equip = {}

    slot_names = {
        1: "UpperArmor",
        2: "LowerArmor",
        3: "ShoulderArmor",
        4: "Sword",
        5: "Shield"
    }

    for row in rows:
        item = {
            'code': row['item_code'],
            'name': row['name'],
            'type': row['type'],
            'modTarget': row['modTarget'],
            'description': row['description'],
            'price': row['price'],
            'value': row['value'],
            'equip': row['equip']
        }

        # equip 필드가 장착 슬롯이면 equip dict에 넣음
        if item['equip'] in slot_names:
            slot = slot_names[item['equip']]
            equip[slot] = item
        else:
            # 그 외는 무조건 inventory로
            inventory.append(item)

    money = 1000  # 예시 값

    conn.close()
    return jsonify({
        "inventory": inventory,
        "equip": equip,
        "money": money
    })


# ✅ 스켈레탈 메시 저장
@app.route('/save_skeletal', methods=['POST'])
def save_skeletal():
    data = request.get_json()
    
    username = data.get('username')
    skeletal = data.get('skeletal', [])

    conn = get_conn()
    cursor = conn.cursor()

    cursor.execute('''
        REPLACE INTO skeletal_data (
            username, mesh1, mesh2, mesh3, mesh4, mesh5
        ) VALUES (%s, %s, %s, %s, %s, %s)
    ''', (
        username,
        skeletal[0] if len(skeletal) > 0 else '',
        skeletal[1] if len(skeletal) > 1 else '',
        skeletal[2] if len(skeletal) > 2 else '',
        skeletal[3] if len(skeletal) > 3 else '',
        skeletal[4] if len(skeletal) > 4 else ''
    ))

    conn.commit()
    conn.close()
    return jsonify({'success': True})


# ✅ 스켈레탈 메시 불러오기
@app.route('/load_skeletal', methods=['GET'])
def load_skeletal():
    username = request.args.get('username')
    conn = get_conn()
    cursor = conn.cursor(pymysql.cursors.DictCursor)  # DictCursor 사용

    cursor.execute('SELECT * FROM skeletal_data WHERE username = %s', (username,))
    row = cursor.fetchone()

    skeletal = []
    if row:
        skeletal = [
            row['mesh1'],
            row['mesh2'],
            row['mesh3'],
            row['mesh4'],
            row['mesh5']
        ]

    conn.close()
    return jsonify({"skeletal": skeletal})


# ✅ 서버 실행
if __name__ == '__main__':
    app.run(debug=True)
