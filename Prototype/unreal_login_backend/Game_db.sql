
CREATE DATABASE game_db;
USE game_db;

CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);


CREATE TABLE skeletal_data (
    username VARCHAR(50) PRIMARY KEY,
    mesh1 VARCHAR(100),
    mesh2 VARCHAR(100),
    mesh3 VARCHAR(100),
    mesh4 VARCHAR(100),
    mesh5 VARCHAR(100)
);

CREATE TABLE inventory (
    username VARCHAR(50),
    item_code INT,
    name VARCHAR(100),
    type VARCHAR(50),
    modTarget VARCHAR(50),
    description TEXT,
    price INT,
    value INT,
    equip INT
);

CREATE TABLE stats (
    username VARCHAR(50) PRIMARY KEY,
    level INT,
    maxHp INT,
    curHp INT,
    maxMp INT,
    curMp INT,
    str_ INT,
    dex INT,
    int_ INT,
    exp INT,
    nextExp INT,
    bonusPoint INT,
    attackRadius FLOAT,
    attackRange FLOAT
);

