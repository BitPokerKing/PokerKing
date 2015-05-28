#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m_socket_id = -1;
int myjetton, mymoney;
int otherjetton[8];
int othermoney[8];
int aliveplayer = 8;

/* ����server����Ϣ */
int on_server_message(int length, const char* buffer)
{
	printf("Recieve Data From Server(%s)\n", buffer);

	return 0;
}

int toint(char *num)
{
	int s = 0;
	int len = strlen(num);
	for (int i = 0; i < len; i++)
	{
		s = s * 10;
		s += (num[i] - '0');
	}
	return s;
}

int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		printf("Usage: ./%s server_ip server_port my_ip my_port my_id\n", argv[0]);
		return -1;
	}

	/* ��ȡ������� */
	in_addr_t server_ip = inet_addr(argv[1]);
	in_port_t server_port = htons(atoi(argv[2]));
	in_addr_t my_ip = inet_addr(argv[3]);
	in_port_t my_port = htons(atoi(argv[4]));
	int my_id = atoi(argv[5]);
	
	char myid[10];
	strcpy(myid, argv[5]);
	
		/* ����socket */
	m_socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket_id < 0)
	{
		printf("init socket failed!\n");
		return -1;
	}

	/* ����socketѡ���ַ�ظ�ʹ�ã���ֹ����������˳����´�����ʱbindʧ�� */
	int is_reuse_addr = 1;
	setsockopt(m_socket_id, SOL_SOCKET, SO_REUSEADDR, (const char*)&is_reuse_addr, sizeof(is_reuse_addr));

	/* ��ָ��ip��port����Ȼ�ᱻserver�ܾ� */
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = my_ip;
	my_addr.sin_port = my_port;
	if (bind(m_socket_id, (struct sockaddr*)&my_addr, sizeof(my_addr)))
	{
		printf("bind failed!\n");
		return -1;
	}

	/* ����server */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = server_ip;
	server_addr.sin_port = server_port;
	while (connect(m_socket_id, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		usleep(100 * 1000); /* sleep 100ms, Ȼ�����ԣ���֤����server�����Ǻ��𣬶����������� */
	}

	/* ��serverע�� */
	char reg_msg[50] = { '\0' };
	snprintf(reg_msg, sizeof(reg_msg)-1, "reg: %d %s \n", my_id, "PokerKing");
	send(m_socket_id, reg_msg, strlen(reg_msg) + 1, 0);

	/* ����server��Ϣ��������Ϸ */
	int now, Situ=0;//last��¼��ǰ��Ϣ״����Ϊseat,inquire֮��
	char words[10][10];//��ȡÿ�е���Ϣ
	int num;//��ʾһ����Ϣ�е�����
	int count = 0;
	int lastbet;
	char action[10];
	while (1)
	{
		char buffer[1024] = { '\0' };
		int length = recv(m_socket_id, buffer, sizeof(buffer)-1, 0);
		if (length > 0)
		{
			/* on_server_message����-1�������յ�game over��Ϣ����������ѭ�����ر�socket����ȫ�˳����� */
			if (-1 == on_server_message(length, buffer))
			{
				break;
			}

			//�����յ�����Ϣ
			int i;
			count = 0;
			for (now = 0; now < length; now++)
			{
				if (buffer[now] != '\n')	//���д�����Ϣ
				{
					if (buffer[now] != ' ')
					{
						for (i = 0; buffer[now + i] != ' '; i++)
						{
							words[count][i] = buffer[now + i];
						}
						now += i;
						count++;
					}
				}
				else
				{
					//����һ�����з�˵��һ�ж�ȡ��ϣ���������������
					if (Situ == 0)
					{
						if (count == 1 && strcmp(words[0], "seat/") == 0)
						{
							Situ = 1;	//��¼��λ�������˵ĳ���ͽ�Ǯ
						}
						else if (count == 1 && strcmp(words[0], "blind/") == 0)
						{
							break;
						}
						else if (count == 1 && strcmp(words[0], "hold/") == 0)
						{
							Situ = 2;
						}
						else if (count == 1 && strcmp(words[0], "inquire/") == 0)
						{
							Situ = 3;
						}
						else if (count == 1 && strcmp(words[0], "flop/") == 0)
						{
							Situ = 4;
						}
						else if (count == 1 && strcmp(words[0], "turn/") == 0)
						{
							Situ = 5;
						}
						else if (count == 1 && strcmp(words[0], "river/") == 0)
						{
							Situ = 6;
						}
						num = 0;
					}
					else if (Situ == 1)
					{
						if (strcmp("button:", words[0]) == 0)
						{
							if (strcmp(myid, words[1]) == 0)
							{
								myjetton = toint(words[2]);
								mymoney = toint(words[3]);
							}
							else
							{
								otherjetton[num] = toint(words[2]);
								othermoney[num] = toint(words[3]);
							}
						}
						else if (strcmp("small", words[0])==0)
						{
							if (strcmp(myid, words[2]) == 0)
							{
								myjetton = toint(words[3]);
								mymoney = toint(words[4]);
							}
							else
							{
								otherjetton[num] = toint(words[3]);
								othermoney[num] = toint(words[4]);
							}
						}
						else if (strcmp("big", words[0]) == 0)
						{
							if (strcmp(myid, words[2]) == 0)
							{
								myjetton = toint(words[3]);
								mymoney = toint(words[4]);
							}
							else
							{
								otherjetton[num] = toint(words[3]);
								othermoney[num] = toint(words[4]);
							}
						}
						else if (strcmp("/seat", words[0]) && count == 1)
						{
							aliveplayer = num;
							Situ = 0;
						}
						else
						{
							if (strcmp(myid, words[0]) == 0)
							{
								myjetton = toint(words[1]);
								mymoney = toint(words[2]);
							}
							else
							{
								otherjetton[num] = toint(words[1]);
								othermoney[num] = toint(words[2]);
							}
						}
					}
					else if (Situ == 2)	
					{
						if (count == 1 && strcmp("/hold", words[0]) == 0)
						{
							Situ = 0;
						}
						else if (count == 2)
						{
							//�޸��Լ����Ƽ���
						}
					}
					else if (Situ == 3)
					{
						if (count == 1 && strcmp("/inquire", words[0]))
						{
							Situ = 0;
							//����action
						}
						else if (strcmp("fold", words[4]) != 0)
						{
							lastbet = toint(words[3]);
							//���߲���
						}
					}
					else if (Situ == 4)
					{
						if (count == 1 && strcmp("/flop", words[0]))
						{
							Situ = 0;
						}
						else
						{
							//�޸�3�Ź����Լ���ǰ״̬����
						}
					}
					else if (Situ == 5)
					{
						if (count == 1 && strcmp("/turn", words[0]))
						{
							Situ = 0;
						}
						else
						{
							//�޸ĵ�4�Ź����Լ���ǰ״̬����
						}
					}
					else if (Situ == 6)
					{
						if (count == 1 && strcmp("/river", words[0]))
						{
							Situ = 0;
						}
						else
						{
							//�޸ĵ�5�Ź����Լ���ǰ״̬����
						}
					}
					count = 0;
					num++;
				}
			}
		}
	}

	/* �ر�socket */
	close(m_socket_id);

	return 0;
}