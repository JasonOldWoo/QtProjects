#!/bin/sh

passwd="dcs.1234"
read -p "Enter root user password(default dcs.1234): " REPLAY
if [ ! -z $REPLAY ];
then
	passwd=$REPLAY
fi

echo "Install lanc_db.............."
mysql -uroot -p$passwd < lanc_db_sql.sql
