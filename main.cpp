#include <iostream>
#include <string.h>

using namespace std;

char filehead[]="#VRML V2.0 utf8\n\
Shape {\n\
geometry IndexedFaceSet {\n\
ccw  TRUE\n\
solid  TRUE\n\
convex TRUE\n\
coord Coordinate {\n\
point  [\n";

char filevector[]="]}\n\
normal Normal {\n\
vector  [\n";

char filecoordIndex[]="]}\n\
normalPerVertex TRUE\n\
coordIndex [\n";

char filenormalIndex[]="]\n\
normalIndex [\n";

char filetail[]="]}}";

int main(int argc,char *argv[])
{
	cout<<"-------===Pro/E����wrl���򼰺ϲ�Shape����===-------\n";
	cout<<endl;
	cout<<"-˵����Pro/E���ɵ�wrl���н϶�������Ϣ���Ҹ����汻��ֵ����Shape�У����������Ҫ���������ʮ�ַ������ʱ�д������\n";
	cout<<" ������ɺ����ж���ڵ㼰Ƕ�׹�ϵ������ʧ������������ϲ���һ��Shape�ڵ��ϡ�\n";
	cout<<"-���ߣ�Tom Willow 2016.05.12\n";
	cout<<"-ʹ�÷������϶�wrl�ļ�����exe����ͼ�꼴�ɡ�\n";
	cout<<endl;
	if (argc!=2)
	{
		cout<<"Warning:�뽫�������ļ��ϵ���exe����ͼ���ϡ�\n";
		cout<<"Warning:�����ļ������ݽ���ֱ�Ӹ��ǡ�\n";
		getchar();
		exit(0);
	}

	cout<<"---------------------------------------------------"<<endl;
	cout<<">>�����ļ���"<<argv[1]<<endl<<endl;

	FILE *fp;
	fp=fopen(argv[1],"r");
	
	if (fp==NULL)
	{
		cout<<"Error:�ļ���ȡʧ�ܡ�\n";
		getchar();
		exit(0);
	}
	int originsize=0;
	fseek(fp,0,SEEK_END);
	originsize=ftell(fp);
	rewind(fp);

	int newsize=0;
	char buf[1024];
	int buflen=0;

	//Ϊ4���η���ԭ�ļ���С���ڴ�ռ�
	char *point=(char *)malloc(originsize);
	(*point)='\0';
	char *vector=(char *)malloc(originsize);
	(*vector)='\0';
	char *coordIndex=(char *)malloc(originsize);
	(*coordIndex)='\0';
	char *normalIndex=(char *)malloc(originsize);
	(*normalIndex)='\0';
	int pointlen=0;
	int vectorlen=0;
	int coordIndexlen=0;
	int normalIndexlen=0;
	bool pointshow=false;
	bool vectorshow=false;
	bool coordIndexshow=false;
	bool normalIndexshow=false;
	
	char c;
	while ((c=getc(fp))!=EOF)
	{
		//cout<<c;
		switch (c)
		{
		case '\n':
			//�㷨���������point�������ж��Ƿ�point��ĩβ����ĩβ����point������
			//      δ��ĩβ���ƻ�����buf��point����δ����point�����жϸ����Ƿ�Ϊpoint����ʼ�С�

			///point
			if (pointshow)
			{
				if (strstr(buf,"]"))
				{
					pointshow=false;
				}
				else
				{
					strcat(point,buf);
					strcat(point,"\n");
					pointlen+=strlen(buf)+1;
				}
			}
			else
			{
				if (strstr(buf,"point")&&strstr(buf,"["))
				{
					pointshow=true;
				}
			}

			///vector
			if (vectorshow)
			{
				if (strstr(buf,"]"))
				{
					vectorshow=false;
				}
				else
				{
					strcat(vector,buf);
					strcat(vector,"\n");
					vectorlen+=strlen(buf)+1;
				}
			}
			else
			{
				if (strstr(buf,"vector")&&strstr(buf,"["))
				{
					vectorshow=true;
				}
			}

			///coordIndex
			if (coordIndexshow)
			{
				if (strstr(buf,"]"))
				{
					coordIndexshow=false;
				}
				else
				{
					strcat(coordIndex,buf);
					strcat(coordIndex,"\n");
					coordIndexlen+=strlen(buf)+1;
				}
			}
			else
			{
				if (strstr(buf,"coordIndex")&&strstr(buf,"["))
				{
					coordIndexshow=true;
				}
			}

			///normalIndex
			if (normalIndexshow)
			{
				if (strstr(buf,"]"))
				{
					normalIndexshow=false;
				}
				else
				{
					strcat(normalIndex,buf);
					strcat(normalIndex,"\n");
					normalIndexlen+=strlen(buf)+1;
				}
			}
			else
			{
				if (strstr(buf,"normalIndex")&&strstr(buf,"["))
				{
					normalIndexshow=true;
				}
			}
			buf[0]='\0';
			buflen=0;
			break;
		default:
			buf[buflen]=c;
			buflen++;
			buf[buflen]='\0';
			break;
		}
	}
	//originsize=ftell(fp);
	fclose(fp);

	fp=fopen(argv[1],"w");
	fwrite(filehead,1,sizeof(filehead)-1,fp);
	fwrite(point,1,pointlen,fp);
	fwrite(filevector,1,sizeof(filevector)-1,fp);
	fwrite(vector,1,vectorlen,fp);
	fwrite(filecoordIndex,1,sizeof(filecoordIndex)-1,fp);
	fwrite(coordIndex,1,coordIndexlen,fp);
	fwrite(filenormalIndex,1,sizeof(filenormalIndex)-1,fp);
	fwrite(normalIndex,1,normalIndexlen,fp);
	fwrite(filetail,1,sizeof(filetail)-1,fp);
	newsize=ftell(fp);
	fclose(fp);

	cout<<">>������ɣ�"<<endl;
	cout<<">>����ǰ"<<originsize/1024<<"."<<originsize%1024/100;
	cout<<"KB�������"<<newsize/1024<<"."<<newsize%1024/100<<"KB"<<endl;
	cout<<">>��������˳�...";
	getchar();
}