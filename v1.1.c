#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DEL -1
#define LONG 65536

unsigned int hasher(char a[]);

typedef struct node{
	char name[256];
	int dir;
	int load;
	struct node *sub[256];
	char content[256];	
	struct node *nextincluster;
	} node;

typedef struct found{
	struct found *before;
	char path[LONG];
	struct found *after;
} found;

int delete(char path[],node*start);
int create_dir(char path[],node* start);
int create(char path[],node* start);
int delete_dir(char path[],node *start);
char *reader(char path[],node *start);
int writer(char path[],node *start,char data[]);
int finder(char stringtofind[], node *start, found *head);
int insert(found *toinsert, found* head);
int cutter(char a[],char x,int max);
int compareall(char a[],char b[]);
int comparen(char a[],char b[],int n);

int main(){
	char megaline[65810]="",command[16]="",path[65536]="",content[256]="",*stringread=NULL;
	int init,res=0,lastsizeofpath=0,place=0,lastsizeofcommand=0,point=0,lastsizeofcontent=0;
	node root;
	found *headoflist=NULL;
	strcpy(root.name,"root");
	for(init=0;init<256;init++){
		root.sub[init]=NULL;
	}
	root.load=0;		
	while(compareall(command,"exit")!=0){
		place=0;
		point=0;
		fgets(megaline,65808,stdin);
		while(megaline[place]!=' ' && megaline[place]!='\n'){
			command[point]=megaline[place];
			place++;
			point++;
		}
		
		command[point]='\0';
		if(point<lastsizeofcommand){
			while(point<=lastsizeofcommand){
				command[point]='\0';
				point++;
			}
		}
		lastsizeofcommand=place;
		while(megaline[place]==' '){
			place++;
		}
		if(comparen(command,"find",4)==0){
			point=0;
			while(megaline[place]!='\n'){
				content[point]=megaline[place];
				point++;
				place++;
			}
			content[point]='\0';
			if(point<lastsizeofcontent){
				point++;
				while(point<=lastsizeofcontent){
					content[point]='\0';
					point++;
				}
			}
			lastsizeofcontent=strlen(content);
			headoflist=malloc(sizeof(found));
			strcpy(headoflist->path,"");
			headoflist->before=headoflist;
			headoflist->after=NULL;
			if(finder(&content[0],&root,headoflist)==0){
				if(headoflist->after==NULL){
					printf("no\n");
				}
				else{
					while(headoflist->after!=NULL){
						printf("ok %s\n",headoflist->after->path);
						headoflist=headoflist->after;
						free(headoflist->before);
					}
					free(headoflist);
				}
			}
		}
		else{
			if(comparen(command,"write",5)==0){
				point=0;
				while(megaline[place]!=' '){
					path[point]=megaline[place];
					point++;
					place++;
				}
				path[point]='\0';
				if(point<lastsizeofpath){
					point++;
					while(point<=lastsizeofpath){
						path[point]='\0';
						point++;
					}
				}
				lastsizeofpath=strlen(path);
				point=0;
				while(megaline[place]!='"'){
					place++;
				}
				place++;
				while(megaline[place]!='"' && megaline[place]!='\n'){
					content[point]=megaline[place];
					point++;
					place++;
				}
				content[point]='\0';
				if(point<lastsizeofcontent){
					point++;
					while(point<=lastsizeofcontent){
						content[point]='\0';
						point++;
					}
				}
				lastsizeofcontent=strlen(content);	
				res=writer(&path[0],&root,&content[0]);
				if(res>0){
					printf("ok %d\n",res);
				}
				else{
					printf("no\n");
				}
			}
			else{
				point=0;
				while(megaline[place]!='\n'){
					path[point]=megaline[place];
					point++;
					place++;
				}
				path[point]='\0';
				if(point<lastsizeofpath){
					point++;
					while(point<=lastsizeofpath){
						path[point]='\0';
						point++;
			        }
			    }
			    lastsizeofpath=strlen(path);
	    		if(comparen("read",command,4)==0){
	    			stringread=reader(&path[0],&root);
	    			if(stringread!=NULL){
	    				printf("contenuto %s\n",stringread);
					}
					else{
						printf("no\n");
					}
	    		}
	    		else{
					if(comparen("create_dir",command,10)==0){
						if(create_dir(&path[0],&root)==0){
		    				printf("ok\n");
						}
						else{
							printf("no\n");
						}			
		    		}
		    		else{
		    			if(comparen("create",command,6)==0){
		    				if(create(&path[0],&root)==0){
		    					printf("ok\n");
							}
							else{
								printf("no\n");
							}
		    			}
		    			else{
			    			if(comparen("delete_r",command,8)==0){
				    			if(delete_dir(&path[0],&root)==0){
				    				printf("ok\n");
				    			}
				    			else{
				    				printf("no\n");
				    			}
				    		}
				    		else{
				    			if(comparen("delete",command,6)==0){
				    				if(delete(&path[0],&root)==0){
				    					printf("ok\n");
				    				}
				    				else{
								    	printf("no\n");
							    	}
						    	}
						    	else{
						    		if(comparen("exit",command,4)==0){
										return 0;
									}
									else{
										printf("error, command not recognized");
										scanf("%d",res);
										return -1;
									}
								}
					    	}
						}
		    			
		    		}
				}
	    	}
		}	
	}
	return 0;
}

unsigned hasher(char a[]){
	int i;				
	unsigned int hash=71;
	for(i=0;i<strlen(a);i++){				
		hash=((hash*33)+a[i])%256;
	}
	return hash;
}

int create_dir(char path[],node *start){
	char step[256];
	int endofpath=0,end;
	int limitforcopy,contatore;
	node *son,*cluster=NULL;
	unsigned int hashed;
	end=cutter(&path[1],'/',strlen(path));
	if(end==-1){
		endofpath=1;
		end=cutter(&path[1],'\0',(strlen(path)+1));
	}
	strncpy(step,&path[1],end);
	step[end]='\0';
	limitforcopy=end;
	hashed=hasher(&step[0]);
	if(endofpath){
		if(start->load>1024){
			return 1;
		}	
		if(start->sub[hashed]!=NULL){
			if(compareall(start->sub[hashed]->name,step)==0){
				return 1;
			}
			else{
				cluster=start->sub[hashed];
				while(cluster->nextincluster!=NULL){
					if(compareall(cluster->nextincluster->name,step)==0){
						return 1;
					}
					cluster=cluster->nextincluster;
				}
				start->load=start->load+1;
				son=malloc(sizeof(node));
				son->dir=1;
				for(contatore=0;contatore<256;contatore++){
					son->sub[contatore]=NULL;
				}
				son->load=0;
				strcpy(son->name,step);
				cluster->nextincluster=son;
				son->nextincluster=NULL;
				return 0;							
			}
		}
		else{
			start->load=start->load+1;
			son=malloc(sizeof(node));
			son->dir=1;
			for(contatore=0;contatore<256;contatore++){
				son->sub[contatore]=NULL;
				}
			son->load=0;
			strcpy(son->name,step);
			start->sub[hashed]=son;
			son->nextincluster=NULL;
			return 0;
		}
	}
	else{
		if(start->load==0){
			return -1;
		}
		if(start->sub[hashed]!=NULL){
			if(compareall(step,start->sub[hashed]->name)!=0){
				cluster=start->sub[hashed];
				while(cluster->nextincluster!=NULL){
					if(compareall(step,cluster->nextincluster->name)==0){
						if(cluster->nextincluster->dir==1){
							return create_dir(&path[limitforcopy+1],start->sub[hashed]);
						}
						else{
							return 1;
						}
					}
					else{
						cluster=cluster->nextincluster;
					}
				}
			}
			else{
				if(start->sub[hashed]->dir==1){
					return create_dir(&path[limitforcopy+1],start->sub[hashed]);
				}
				else{
					return 1;	
				}
			}
		}
		else{
			return 1;
		}
	}
	return 1;
}

int create(char path[],node *start){
	char step[256];
	int endofpath=0,end;
	int limitforcopy;
	node *son,*cluster;
	unsigned int hashed;
	end=cutter(&path[1],'/',strlen(path));
	if(end==-1){
		endofpath=1;
		end=cutter(&path[1],'\0',(strlen(path)+1));
	}
	strncpy(step,&path[1],end);
	step[end]='\0';
	limitforcopy=end;
	hashed=hasher(&step[0]);
	if(endofpath){
		if(start->load==1024){
			return 1;
		}
		if(start->sub[hashed]!=NULL){
			if(compareall(start->sub[hashed]->name,step)==0){
					return 1;
				}
			else{
				cluster=start->sub[hashed];
				while(cluster->nextincluster!=NULL){
					if(compareall(cluster->nextincluster->name,step)==0){
						return 1;
					}
					cluster=cluster->nextincluster;
				}
				start->load=start->load+1;
				son=malloc(sizeof(node));
				son->dir=0;
				strcpy(son->name,step);
				cluster->nextincluster=son;
				return 0;
			}
		}
		else{
			start->load=start->load+1;
			son=malloc(sizeof(node));
			son->dir=0;
			strcpy(son->name,step);
			son->nextincluster=NULL;
			start->sub[hashed]=son;
			return 0;
		}
	}
	else{
		if(start->load==0){
			return -1;
		}
		if(start->sub[hashed]!=NULL){
			if(compareall(step,start->sub[hashed]->name)!=0){
				cluster=start->sub[hashed];
				while(cluster->nextincluster!=NULL){
					if(compareall(step,cluster->nextincluster->name)==0){
						if(cluster->nextincluster->dir==1){
							return create(&path[limitforcopy+1],cluster->nextincluster);
						}
						else{
							return 1;
						}
					}
					else{
						cluster=cluster->nextincluster;
					}				
				}
				return 1;
			}
			else{
				if(start->sub[hashed]->dir==1){
					return create(&path[limitforcopy+1],start->sub[hashed]);
				}
				else{
				return -1;
				}
			}
		}
		else{
			return -1;
		}
	}	
	return 0;
}

int delete(char path[],node*start){
	char step[256];
	int endofpath=0,limitforcopy,end;
	unsigned int hashed;
	node *nomnom,*cluster;
	end=cutter(&path[1],'/',strlen(path));
	if(end==-1){
		endofpath=1;
		end=cutter(&path[1],'\0',(strlen(path)+1));
	}
	strncpy(step,&path[1],end);
	step[end]='\0';
	limitforcopy=end;
	hashed=hasher(&step[0]);
	if(endofpath){
		if(start->load==0){
			return -1;
		}
		if(start->sub[hashed]!=NULL){
				if(compareall(start->sub[hashed]->name,step)==0){
					if(start->sub[hashed]->dir==1 && start->sub[hashed]->load!=0){
						return 1;
					}
					start->load=start->load-1;
					if(start->sub[hashed]->nextincluster!=NULL){
						strcpy(start->sub[hashed]->content,"");
						nomnom=start->sub[hashed]->nextincluster;
						free(start->sub[hashed]);
						start->sub[hashed]=nomnom;
					}
					else{
						strcpy(start->sub[hashed]->content,"");
						free(start->sub[hashed]);
						start->sub[hashed]=NULL;
					}
					return 0;
				}
				else{
					cluster=start->sub[hashed];
					while(cluster->nextincluster!=NULL){
						if(compareall(cluster->nextincluster->name,step)==0){
							if(cluster->nextincluster->dir==1 && cluster->nextincluster->load!=0){
								return 1;
							}
							start->load=start->load-1;
							if(cluster->nextincluster->nextincluster!=NULL){
								nomnom=cluster->nextincluster;
								strcpy(cluster->nextincluster->content,"");
								cluster->nextincluster=cluster->nextincluster->nextincluster;
								free(nomnom);
							}
							else{
								strcpy(cluster->nextincluster->content,"");
								free(cluster->nextincluster);
								cluster->nextincluster=NULL;
							}
							return 0;
						}
						cluster=cluster->nextincluster;
					}
					return 1;
				}	
		}
		else{
			return 1;
		}
	}
	else{
		if(start->load==0){
			return -1;
		}
		if(start->sub[hashed]!=NULL){
			if(compareall(step,start->sub[hashed]->name)!=0){
				cluster=start->sub[hashed];
				while(cluster->nextincluster!=NULL){
					if(compareall(cluster->nextincluster->name,step)==0){
						return delete(&path[limitforcopy+1],cluster->nextincluster);
					}
					else{
						cluster=cluster->nextincluster;
					}
				}
				return 1;
			}
			else{
				if(start->sub[hashed]->dir==1){
					return delete(&path[limitforcopy+1],start->sub[hashed]);
				}
				else{
					return -1;	
				}
			}
		}
		else{
			return -1;
		}		
	}
}

int delete_dir(char path[],node*start){
	int i;
	char step[256],subpath[257];
	int endofpath=0,limitforcopy,end;
	unsigned int hashed;
	node *nomnom,*cluster;
	strcpy(subpath,"/");
	end=cutter(&path[1],'/',strlen(path));
	if(end==-1){
		endofpath=1;
		end=cutter(&path[1],'\0',(strlen(path)+1));
	}
	strncpy(step,&path[1],end);
	step[end]='\0';
	limitforcopy=end;	
	hashed=hasher(&step[0]);
	if(endofpath){
		if(start->load==0){
			return -1;
		}
		if(start->sub[hashed]!=NULL){
			if(compareall(start->sub[hashed]->name,step)==0){
				if(start->sub[hashed]->dir==1){
					if(start->sub[hashed]->load==0){
						if(start->sub[hashed]->nextincluster!=NULL){
							nomnom=start->sub[hashed];
							start->sub[hashed]=start->sub[hashed]->nextincluster;
							free(nomnom);
						}
						else{
							free(start->sub[hashed]);
							start->sub[hashed]=NULL;
						}
						return 0;
					}
					for(i=0;i<256;i++){
						while(start->sub[hashed]->sub[i]!=NULL){
							if(start->sub[hashed]->sub[i]->dir==0){
								strcat(subpath,start->sub[hashed]->sub[i]->name);
								if(delete(&subpath[0],start->sub[hashed])!=0){
									return -1;
								}
								strcpy(subpath,"/");
							}
							else{
								strcat(subpath,start->sub[hashed]->sub[i]->name);
								if(delete_dir(&subpath[0],start->sub[hashed])!=0){
										return -1;
								}
			             		strcpy(subpath,"/");
							}	
						}
					}
					start->load=start->load-1;
					if(start->sub[hashed]->nextincluster==NULL){
						free(start->sub[hashed]);
						start->sub[hashed]=NULL;
					}
					else{
						nomnom=start->sub[hashed];
						start->sub[hashed]=start->sub[hashed]->nextincluster;
						free(nomnom);
					}
					return 0;
				}
				else{
					start->load=start->load-1;
					if(start->sub[hashed]->nextincluster==NULL){
						free(start->sub[hashed]);
						start->sub[hashed]=NULL;
					}
					else{
						nomnom=start->sub[hashed];
						start->sub[hashed]=start->sub[hashed]->nextincluster;
						free(nomnom);
					}
					return 0;
				}
			}
			else{
				if(start->sub[hashed]->nextincluster!=NULL){
					cluster=start->sub[hashed];
					while(cluster->nextincluster!=NULL){
						if(compareall(cluster->nextincluster->name,step)==0){
							if(cluster->nextincluster->dir==1){
								for(i=0;i<256;i++){
									if(cluster->nextincluster->sub[i]->dir==0){
										strcat(subpath,cluster->nextincluster->sub[i]->name);
										if(delete(subpath,cluster->nextincluster)!=0){
											return 1;
										}
										strcpy(subpath,"/");
									}
									else{
										strcat(subpath,cluster->nextincluster->sub[i]->name);
										if(delete_dir(subpath,cluster->nextincluster)!=0){
											return 1;
										}
										strcpy(subpath,"/");
									}
								}
								if(cluster->nextincluster->nextincluster!=NULL){
									nomnom=cluster->nextincluster;
									cluster->nextincluster=cluster->nextincluster->nextincluster;
									free(nomnom);
								}
								else{
									free(cluster->nextincluster);
									cluster->nextincluster=NULL;
								}
								return 0;
							}
							else{
								if(cluster->nextincluster->nextincluster!=NULL){
									nomnom=cluster->nextincluster;
									cluster->nextincluster=cluster->nextincluster->nextincluster;
									free(nomnom);
								}
								else{
									free(cluster->nextincluster);
									cluster->nextincluster=NULL;
								}
								return 0;
							}
						}
					}
					return 1;
				}
				else{
					return 1;
				}
			}
		}
		else{
			return 1;
		}
	}
	else{
		if(start->load==0){
			return -1;
		}
		if(start->sub[hashed]!=NULL){
			if(compareall(step,start->sub[hashed]->name)!=0){
				if(start->sub[hashed]->nextincluster!=NULL){
					cluster=start->sub[hashed];
					while(cluster->nextincluster!=NULL){
						if(compareall(step,cluster->nextincluster->name)==0){
							if(cluster->nextincluster->dir==1){
								return delete_dir(&path[limitforcopy+1],cluster->nextincluster);
							}
						}
						cluster=cluster->nextincluster;
					}
					return 1;
				}			
			}
			else{
				if(start->sub[hashed]->dir==1){
					return delete_dir(&path[limitforcopy+1],start->sub[hashed]);
				}
				else{
					
					return 1;	
				}
			}
		}
		else{	
			return -1;
		}
	}
}

int writer(char path[],node *start,char data[]){
	char step[256];
	int endofpath=0,end;
	int limitforcopy,hashed;
	node *cluster;
	end=cutter(&path[1],'/',strlen(path));
	if(end==-1){
		endofpath=1;
		end=cutter(&path[1],'\0',(strlen(path)+1));
	}
	strncpy(step,&path[1],end);
	step[end]='\0';
	limitforcopy=end;
	hashed=hasher(&step[0]);
	if(endofpath){
		if(start->sub[hashed]!=NULL){
			if(compareall(start->sub[hashed]->name,step)==0){
				if(start->sub[hashed]->dir==1){
					return -1;
				}
				else{
					strcpy(start->sub[hashed]->content,data);
					return strlen(start->sub[hashed]->content);					
				}
			}
			else{
				if(start->sub[hashed]->nextincluster!=NULL){
					cluster=start->sub[hashed];
					while(cluster->nextincluster!=NULL){
						if(compareall(cluster->nextincluster->name,step)==0){
							if(cluster->nextincluster->dir==0){
								strcpy(cluster->nextincluster->content,data);
								return strlen(cluster->nextincluster->content);
							}
							else{
								return -1;
							}
						}
						cluster=cluster->nextincluster;
					}
				}
			}
		}
	return -1;	
	}
	else{
		if(start->sub[hashed]!=NULL){
			if(compareall(start->sub[hashed]->name,step)==0){
				if(start->sub[hashed]->dir==1){
					return writer(&path[limitforcopy+1],start->sub[hashed],&data[0]);
				}
				else{
					return -1;
				}
			}
			else{
				if(start->sub[hashed]->nextincluster!=NULL){
					cluster=start->sub[hashed];
					while(cluster->nextincluster!=NULL){
						if(compareall(cluster->nextincluster->name,step)==0){
							return writer(&path[limitforcopy+1],cluster->nextincluster,&data[0]);
						}
						cluster=cluster->nextincluster;
					}
					return 1;
				}
			}
		}
		return -1;	
	}
}

char *reader(char path[],node *start){
	char step[256];
	int endofpath=0,end;
	int limitforcopy,hashed;
	node *cluster;
	end=cutter(&path[1],'/',strlen(path));
	if(end==-1){
		endofpath=1;
		end=cutter(&path[1],'\0',(strlen(path)+1));
	}
	strncpy(step,&path[1],end);
	step[end]='\0';
	limitforcopy=end;
	hashed=hasher(&step[0]);
	if(endofpath){
		if(start->sub[hashed]!=NULL){
			if(compareall(start->sub[hashed]->name,step)==0){
				if(start->sub[hashed]->dir==0){
					return start->sub[hashed]->content;
				}
				else{
					return NULL;	
				}
			}
			else{
				if(start->sub[hashed]->nextincluster!=NULL){
					cluster=start->sub[hashed];
					while(cluster->nextincluster!=NULL){
						if(compareall(cluster->nextincluster->name,step)==0){
							if(cluster->nextincluster->dir==0){
								return cluster->nextincluster->content;
							}
							else{
								return NULL;
							}
						}
						cluster=cluster->nextincluster;
					}
					return NULL;
				}
			}
		}
		return NULL;
	}
	else{
		if(start->sub[hashed]!=NULL){
			if(compareall(start->sub[hashed]->name,step)!=0){
				if(start->sub[hashed]->nextincluster!=NULL){
					cluster=start->sub[hashed];
					while(cluster->nextincluster!=NULL){
						if(compareall(cluster->nextincluster->name,step)==0){
							if(cluster->nextincluster->dir==1){
								return reader(&path[limitforcopy+1],cluster->nextincluster);
							}
							else{
								return NULL;
							}
						}
						cluster=cluster->nextincluster;
					}
				}
				return NULL;
			}
			else{
				if(start->sub[hashed]->dir==1){
					return reader(&path[limitforcopy+1],start->sub[hashed]);
				}
				return NULL;
			}
		}
		return NULL;
	}
}

int finder(char stringtofind[],node *start,found *head){
	int i=0;
	char path[LONG];
	found *occurrence;
	node *cluster;
	strcat(head->path,"/");
	strcpy(path,head->path);
	if(start->load==0){
		return 0;
	}
	for(i=0;i<256;i++){
		cluster=start->sub[i];
		while(cluster!=NULL){
			strcat(head->path,cluster->name);
			if(compareall(cluster->name,stringtofind)==0){
				occurrence=malloc(sizeof(found));
				strcpy(occurrence->path,head->path);
				if(insert(occurrence,head)!=0){
					return -1;
				}
				if(cluster->dir==1){
					if(finder(&stringtofind[0],cluster,head)!=0){
						return -1;
					}
				}
			}
			else{
				if(cluster->dir==1){
					if(finder(&stringtofind[0],cluster,head)!=0){
						return -1;
					}
				}
			}
			cluster=cluster->nextincluster;
			strcpy(head->path,path);
		}
	}
	return 0;
}

int insert(found *toinsert,found *head){
	found *slot;
	int a,b;
	slot=head;
	while(slot->after!=NULL){
		a=0;
		b=0;
		while(toinsert->path[a]==slot->after->path[b] && slot->after->path[b]!='\0'){
			a++;
			b++;
		}
		if(toinsert->path[a]=='\0'){
			if(toinsert->path[b]=='\0'){
				slot=slot->after;
			}
			else{
				slot->after->before=toinsert;
				toinsert->after=slot->after;
				toinsert->before=slot;
				slot->after=toinsert;
				return 0;
			}
		}
		else{
			if(slot->after->path[b]=='\0'){
				slot=slot->after;
			}
			else{
				if(toinsert->path[a]<slot->after->path[b]){
					slot->after->before=toinsert;
					toinsert->after=slot->after;
					toinsert->before=slot;
					slot->after=toinsert;
					return 0;
				}
				else{
					slot=slot->after;
				}
			}
		}
	}
	toinsert->before=slot;
	slot->after=toinsert;
	toinsert->after=NULL;
	return 0;
}

int cutter(char a[],char x,int max){
	int i=0;
	if(x=='\0'){
		return strlen(a);
	}
	while(a[i]!=x && i<max && i<strlen(a)){
		i++;
	}
	if(i<max && i<strlen(a)){
		return i;
	}
	else{
		return -1;
	}
}

int compareall(char a[],char b[]){
	int i,j;
	i=0;
	j=0;
	while(a[i]==b[j] && a[i]!='\0'){
		i++;
		j++;
	}
	if(a[i]=='\0'){
		if(b[j]=='\0'){
			return 0;
		}
		else{
			return -1;
		}
	}
	else{
		if(b[j]=='\0'){
			return 1;
		}
		else{
			if(a[i]>b[j]){
				return 1;
			}
			else{
				return -1;
			}
		}
	}
}

int comparen(char a[],char b[],int n){
	int i=0,j=0;
	while(n>0){
		if(a[i]==b[j]){
			if(a[i]=='\0'){
				return 0;
			}
			i++;
			j++;
		}
		else{
			return 1;
		}
	}
	return 0;
}
/* TODO (#1#): Elimina print di debug */
