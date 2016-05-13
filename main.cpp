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
	cout<<"-------===Pro/E生成wrl精简及合并Shape程序===-------\n";
	cout<<endl;
	cout<<"-说明：Pro/E生成的wrl含有较多冗余信息，且各个面被拆分到多个Shape中，赋予材质需要逐面操作，十分繁琐。故编写本程序。\n";
	cout<<" 精简完成后，所有多余节点及嵌套关系都会消失，并将所有面合并到一个Shape节点上。\n";
	cout<<"-作者：Tom Willow 2016.05.12\n";
	cout<<"-使用方法：拖动wrl文件到本exe程序图标即可。\n";
	cout<<endl;
	if (argc!=2)
	{
		cout<<"Warning:请将待处理文件拖到本exe程序图标上。\n";
		cout<<"Warning:拖入文件的内容将被直接覆盖。\n";
		getchar();
		exit(0);
	}

	cout<<"---------------------------------------------------"<<endl;
	cout<<">>拖入文件："<<argv[1]<<endl<<endl;

	FILE *fp;
	fp=fopen(argv[1],"r");
	
	if (fp==NULL)
	{
		cout<<"Error:文件读取失败。\n";
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

	//为4个段分配原文件大小的内存空间
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
			//算法：如果处于point区，则判断是否到point区末尾，到末尾则标记point区结束
			//      未到末尾则复制缓冲区buf至point。如未处于point区则判断该行是否为point区开始行。

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

	cout<<">>处理完成！"<<endl;
	cout<<">>处理前"<<originsize/1024<<"."<<originsize%1024/100;
	cout<<"KB，处理后"<<newsize/1024<<"."<<newsize%1024/100<<"KB"<<endl;
	cout<<">>按任意键退出...";
	getchar();
}