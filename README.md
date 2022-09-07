# Social_Network_Client
## Simple social network server and client
this is the client implementation, the server implementation is in the Social_Network_Server repository.

Registered users can log in to the system, follow other users, post messages and send private messages to other users. 
The server is implemented in two methods – Thread-Per-Client and Reactor pattern. 



### command lines:
#### server:  
mvn compile

for TPC:  
mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.BGSServer.TPCMain" -Dexec.args="<port>"  

for reactor:  
mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.BGSServer.ReactorMain" -Dexec.args="<Num of threads> <port>"  

#### Client:  
make  
./bin/BGSclient <ip> <port>  



### types of messages:  
REGISTER <username> <password> <birthday>  
  register a user to the service. birthday format is dd-mm-yyyy.
  
LOGIN <username> <password> <captcha>  
  login a user to the server. captcha can be 0 or 1, needs to be 1 for successful login.
  
LOGOUT  
  informs the server of a client disconnection.
  
FOLLOW <0/1> <username> (0 to follow, 1 to unfollow)  
  add/remove the user with username to/from his follow list. 
  
POST <my message>  
  post a message. only the user's followers can see the post. The user can tag another user in the post by writing @username.  

PM <username> <my message>  
  send a private message to another user. A user can send a PM to another user only if the other user is following him.  
  
LOGSTAT  
  recieve data on logged in users - the age of every user ,number of posts every user posted, number of every user’s followers, 
  number of users the user is following.  
  
STAT <username1|username2|username3>  
  recieve data on certain users. the list of user names should be in this format - username1|username2|username3.  
  
BLOCK <username>  
  block onother user. user that is blocked can not send messages to the user that is blocking him and vice versa. 
  also, STAT and LOGSTAT commands don't show data on blocked users.  
  

### filtered words:  
you can make a list of words that would be filterred in private messages. 
we stored the filtered words in the manager class in bgu.spl.net.srv
words can be added in the constructor.  
  
### exapmle:  
CLIENT#1< REGISTER sharon 123 01-08-1997  
CLIENT#1> ACK 1  
CLIENT#1< LOGIN sharon 123 1  
CLIENT#1> ACK 2  
  
CLIENT#2< REGISTER hila pass 01-08-1998  
CLIENT#2> ACK 1  
CLIENT#2< LOGIN hila pass 1  
CLIENT#2> ACK 2  
CLIENT#2< FOLLOW 0 sharon  
CLIENT#2> ACK 4 sharon  

CLIENT#1< POST hello world!  
CLIENT#1> ACK 5  

CLIENT#2> NOTIFICATION Public sharon hello world!  
