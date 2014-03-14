drop database if exists lanc_db;
commit;

create database if not exists lanc_db default character set utf8;
commit;

use lanc_db;
create table users
(
user_id			int unsigned primary key  auto_increment,
user_group_id		int unsigned not null,
user_name			varbinary(129) not null,
user_pwd			varbinary(129) not null,
user_type			int unsigned not null,
user_flag			 smallint unsigned not null,
login_time			time,
login_date			date,
logout_time			time,
logout_date			date,
index user_index(user_id),
unique index user_name(user_id)
)engine=innodb auto_increment=1000 default character set utf8;

create table userGrant
(
id				int unsigned primary key auto_increment,
user_id_a			int unsigned not null,
user_id_b			int unsigned not null,
deal_flag			smallint not null,
unique key(user_id_a, user_id_b),
foreign key(user_id_a) references users(user_id) on delete cascade on update cascade,
foreign key(user_id_b) references users(user_id) on delete cascade on update cascade
)engine=innodb default character set utf8;

create table groups
(
group_id			int unsigned primary key auto_increment,
group_name			varbinary(129) not null,
group_admin		int unsigned not null,
group_type			int unsigned not null,
index group_index(group_id),
unique index group_name(group_id),
foreign key(group_admin) references users(user_id)  on delete cascade on update cascade
)engine=innodb auto_increment=1000 default character set utf8;


grant all privileges on lanc_db.* to 'lancuser'@'localhost'
	identified by 'lanc.webcon' with grant option;

grant all privileges on lanc_db.* to 'lancuser'@'%'
	identified by 'lanc.webcon' with grant option;



insert into users(user_id, user_name, user_pwd, user_type, user_flag) values (1001,'admin', aes_encrypt('123456', 'users.user_pwd'), 0, 0);

insert into groups(group_id, group_name, group_admin) values(1001, 'commongroup', 1001);

update users set user_group_id=1001;

