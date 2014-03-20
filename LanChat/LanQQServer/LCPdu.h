#ifndef LCPDU_H
#define LCPDU_H
#include <QtGlobal>

const quint16 LCDB_UserLogin_Rep_FromCli = 1;
const quint16 LCDB_UserLogin_Rsp_ToCli = 2;
const quint16 LCDB_UserCreate_Rep_FromCli = 3;
const quint16 LCDB_UserCreate_Rsp_ToCli = 4;
const quint16 LCDB_GroupChat_Rep_FromCli = 5;
const quint16 LCDB_GroupChat_Rsp_ToCli = 6;
const quint16 LCDB_GroupMsg_BCast_ToMCli = 7;
const quint16 LCDB_PerChat_Rep_FromCli = 8;
const quint16 LCDB_PerChat_Rsp_ToCli = 9;
const quint16 LCDB_AddFriend_Rep_FromCli = 10;
const quint16 LCDB_AddFriend_Rsp_ToCli = 11;
const quint16 LCDB_AddFriend_Rep_ToCli = 12;
const quint16 LCDB_GetFriendList_Rep_FromCli = 13;
const quint16 LCDB_GetFriendList_Rsp_ToCli = 14;
const quint16 LCDB_KickUser_Req_ToCli = 15;


//=======================================================================
#define LCDB_ERR_SUCCESS	0
#define LCDB_ERR_DBDATA_ERROR		-4001	//数据库错误
#define LCDB_ERR_USER_NotExist		-4002	//用户不存在
#define LCDB_ERR_USER_VerifyFailed	-4003	//用户验证失败（密码、类型错误）
#define LCDB_ERR_USER_Existed		-4004	//用户已存在
#define LCDB_ERR_USER_InBlackList	-4005	//用户处于屏蔽状态
#define LCDB_ERR_USER_Online        -4006   //

#define LC_ERR_USER_OffLine		-5001		//用户处于离线状态
#define LC_ERR_USER_NotFriend		-5002	//非好友


//=======================================================================
#define MAX_OUTBUF_SIZE 1024

#endif // LCPDU_H
