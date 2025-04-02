from flask import Flask, request, jsonify
from flask_cors import CORS
import mysql.connector

app = Flask(__name__)
CORS(app)

# DB 연결 정보
db_config = {
    "host": "localhost",
    "user": "root",
    "password": "your_mysql_password",
    "database": "unreal_login"
}

def get_db():
    return mysql.connector.connect(**db_config)


@app.route("/login", methods=["POST"])
def login():
    data = request.get_json()
    username = data.get("username")
    password = data.get("password")

    conn = get_db()
    cursor = conn.cursor(dictionary=True)
    cursor.execute("SELECT id FROM users WHERE username=%s AND password=%s", (username, password))
    user = cursor.fetchone()

    if user:
        return jsonify(success=True, message="Login successful", user_id=user["id"])
    else:
        return jsonify(success=False, message="Invalid credentials")


@app.route("/save_stats", methods=["POST"])
def save_stats():
    data = request.get_json()
    user_id = data.get("user_id")
    stats = data.get("stats")

    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("SELECT user_id FROM player_stats WHERE user_id=%s", (user_id,))
    exists = cursor.fetchone()

    if exists:
        cursor.execute("""
            UPDATE player_stats
            SET level=%s, max_hp=%s, cur_hp=%s,
                max_mp=%s, cur_mp=%s,
                str_stat=%s, dex=%s, int_stat=%s,
                exp=%s, next_exp=%s, bonus_point=%s,
                money=%s, attack_radius=%s, attack_range=%s
            WHERE user_id=%s
        """, (
            stats["level"], stats["max_hp"], stats["cur_hp"],
            stats["max_mp"], stats["cur_mp"],
            stats["str"], stats["dex"], stats["int"],
            stats["exp"], stats["next_exp"], stats["bonus_point"],
            stats["money"], stats["attack_radius"], stats["attack_range"],
            user_id
        ))
    else:
        cursor.execute("""
            INSERT INTO player_stats
            (user_id, level, max_hp, cur_hp, max_mp, cur_mp, str_stat, dex, int_stat,
             exp, next_exp, bonus_point, money, attack_radius, attack_range)
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)
        """, (
            user_id, stats["level"], stats["max_hp"], stats["cur_hp"],
            stats["max_mp"], stats["cur_mp"],
            stats["str"], stats["dex"], stats["int"],
            stats["exp"], stats["next_exp"], stats["bonus_point"],
            stats["money"], stats["attack_radius"], stats["attack_range"]
        ))

    conn.commit()
    conn.close()
    return jsonify(success=True)


@app.route("/save_inventory", methods=["POST"])
def save_inventory():
    data = request.get_json()
    user_id = data.get("user_id")
    items = data.get("inventory")

    conn = get_db()
    cursor = conn.cursor()

    # 기존 데이터 삭제 후 재삽입
    cursor.execute("DELETE FROM inventory_items WHERE user_id=%s", (user_id,))
    for item in items:
        cursor.execute("""
            INSERT INTO inventory_items (user_id, item_code, item_type, equip_type, quantity)
            VALUES (%s, %s, %s, %s, %s)
        """, (
            user_id,
            item["item_code"],
            item["item_type"],
            item.get("equip_type", 0),
            item.get("quantity", 1)
        ))

    conn.commit()
    conn.close()
    return jsonify(success=True)


@app.route("/save_meshes", methods=["POST"])
def save_meshes():
    data = request.get_json()
    user_id = data.get("user_id")
    meshes = data.get("meshes", [""] * 5)

    conn = get_db()
    cursor = conn.cursor()
    cursor.execute("SELECT user_id FROM player_meshes WHERE user_id=%s", (user_id,))
    exists = cursor.fetchone()

    if exists:
        cursor.execute("""
            UPDATE player_meshes
            SET mesh_0=%s, mesh_1=%s, mesh_2=%s, mesh_3=%s, mesh_4=%s
            WHERE user_id=%s
        """, (*meshes, user_id))
    else:
        cursor.execute("""
            INSERT INTO player_meshes (user_id, mesh_0, mesh_1, mesh_2, mesh_3, mesh_4)
            VALUES (%s, %s, %s, %s, %s, %s)
        """, (user_id, *meshes))

    conn.commit()
    conn.close()
    return jsonify(success=True)


@app.route("/load_data", methods=["GET"])
def load_data():
    user_id = request.args.get("user_id")

    conn = get_db()
    cursor = conn.cursor(dictionary=True)

    # stats
    cursor.execute("SELECT * FROM player_stats WHERE user_id=%s", (user_id,))
    stats = cursor.fetchone()

    # inventory
    cursor.execute("SELECT * FROM inventory_items WHERE user_id=%s", (user_id,))
    inventory = cursor.fetchall()

    # meshes
    cursor.execute("SELECT * FROM player_meshes WHERE user_id=%s", (user_id,))
    mesh_row = cursor.fetchone()
    meshes = [mesh_row[f"mesh_{i}"] for i in range(5)] if mesh_row else []

    conn.close()
    return jsonify(success=True, data={
        "stats": stats,
        "inventory": inventory,
        "meshes": meshes
    })


@app.route("/register", methods=["POST"])
def register():
    data = request.get_json()
    username = data.get("username")
    password = data.get("password")

    conn = get_db()
    cursor = conn.cursor()
    try:
        cursor.execute("INSERT INTO users (username, password) VALUES (%s, %s)", (username, password))
        conn.commit()
        return jsonify(success=True)
    except mysql.connector.errors.IntegrityError:
        return jsonify(success=False, message="Username already exists")
    finally:
        conn.close()


if __name__ == "__main__":
    app.run(debug=True)
