library(dplyr)
maxrows<--1
MACPARA_negUsernum<-5
MACPARA_neguser_labelnum<-2
MACPARA_subsamp<--1
activeDate<-as.Date("2016-03-01")
MACPARA_test_windowday<-as.Date("2016-04-11")
predict_flag<-F
timeSpan<-60
#--------------------------------------------------------------------------------------------------- 
paste(ParentDirectory(),'codes/data preprocess/load_cleandata.R',sep="")%>%source
actionInfo<-actionInfo[cate==8]
paste(ParentDirectory(),'codes/data preprocess/time_mapping.R'  ,sep="")%>%source
paste(ParentDirectory(),'codes/data preprocess/test_as_the_predict_samples.R',sep="")%>%source
#--------------------------------------------------------------------------------------------------- 
start_time<-Sys.time()
#--------------------------------------------------------------------------------------------------- 
User_act_label[,'timegap']<-(User_act_label$labelday-User_act_label$day)%>%as.numeric  
User_act_label<-User_act_label[timegap<=timeSpan]
User_act_label[,'timegap']<-paste('timegap',User_act_label$timegap,'type',sep="_")
User_act_label<-User_act_label[,.(user_id,labelday,label,timegap)] 
User_act_label<-dcast(User_act_label,user_id+labelday+label~timegap,fun=length,value.var=c('timegap'))
#--------------------------------------------------------------------------------------------------- 
write.csv(User_act_label,paste(ParentDirectory(),'save/DNN_feeds.csv',sep=""), row.names = F)
# #------------------------------------------------------------------------------
end_time<-Sys.time()
time_diff<-(end_time%>%as.numeric-start_time%>%as.numeric)%>%round
paste("have finished module: get training feeds [",time_diff,"secs ]")%>%print



