drop database if exists snake_db;
commit;

create database if not exists snake_db default character set utf8;
commit;

use snake_db;
create table score
(
	user_index int unsigned primary key auto_increment,
	user_name varchar(129) not null,
	user_score int unsigned not null
)engine=innodb auto_increment=1000 default character set utf8;

grant all privileges on snake_db.* to 'snakeuser'@'localhost'
	identified by 'snake.webcon' with grant option;

grant all privileges on snake_db.* to 'snakeuser'@'%'
	identified by 'snake.webcon' with grant option;
