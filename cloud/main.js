// Use Parse.Cloud.define to define as many cloud functions as you want.
// For example:
Parse.Cloud.define("hello", function(request, response) {
  response.success("Hello world!");
});
 
//--------------------------------------------------------------------------------
 
Parse.Cloud.define("echo", function(request, response) {
    value = request.params.value;
  response.success(value);
});
 
//--------------------------------------------------------------------------------
 
Parse.Cloud.define("get_username_from_tag", function(request, response) {
    tag = request.params.TAG;   //MCU params are in caps
 
    var getUserId = Parse.Object.extend("Tag"); //search "Tag" table for the user of received tag
    var getUserIdQuery = new Parse.Query(getUserId);
 
    getUserIdQuery.equalTo("rfidTag", tag);
 
    var userID;
 
    getUserIdQuery.first({
        success: function(object) {
            if (typeof(object) == 'undefined') {
                response.error("Not registered");
            }else{
                userID=object.get('userID');   //ger the pointer object not the id value
                //console.log("userID " + JSON.stringify(object.get('userID')));   
        //--------------------------------------------------------------------------------
 
                var getName = Parse.Object.extend("UserInfo"); //search "Tag" table for the user of received tag
                var getNameQuery = new Parse.Query(getName);
 
                getNameQuery.equalTo("userID", userID);
 
                var userName;
 
                getNameQuery.first({
                    success: function(object){
 
                    if(typeof(object) == 'undefined'){
                      }else{
                        userName=object.get('Name') + " " + object.get('LastName');
                        response.success(userName);
                      }
                    },
                    error: function(error){
                        //response.error("Parse error");
                        response.error("get_username_from_tag Public Function error 2:"+error);
                    }
                  });
        //--------------------------------------------------------------------------------
            }
        },
        error: function(error) {
            response.error("get_username_from_tag Public Function error 1:"+error);
        }
    });
});
 
//--------------------------------------------------------------------------------
/*
Parse.Cloud.define("get_userId_pointer_from_tag", function(request, response) {
    tag = request.params.TAG;   //MCU params are in caps
 
    var getUserId = Parse.Object.extend("Tag"); //search "Tag" table for the user of received tag
    var getUserIdQuery = new Parse.Query(getUserId);
 
    getUserIdQuery.equalTo("rfidTag", tag);
 
    var userID;
    var userName;
 
    getUserIdQuery.first({
        success: function(object){
 
            if(typeof(object) == 'undefined'){
                response.error("Tag not registered");
            }else{
 
      //console.log("object " + JSON.stringify(object));  //magic
      //console.log("userID " + JSON.stringify(object.get('userID')));  //magic
      //console.log("userID " + JSON.stringify(object.get('userID').id));  //magic
 
 
                userID=object.get('userID');
 
                //console.log("Result " +  JSON.stringify(userID));
 
                //userName=get_userName_from_userId(object.get('userID'));
 
                //response.success(userID); //tag's userId
                response.success(userID);   //tag's userId
            }
 
        },
        error: function(error){
            response.error("get_userId_from_tag Public Function error:"+error);
 
        }
 
    });
 
});
 
//--------------------------------------------------------------------------------
 
Parse.Cloud.define("get_userName_from_userId_pointer", function(request, response) {
    //userId = request.params.objectId; //MCU params are in caps
    userId = request.params;    //MCU params are in caps
 
 
  var getName = Parse.Object.extend("UserInfo"); //search "Tag" table for the user of received tag
  var getNameQuery = new Parse.Query(getName);
 
  //getNameQuery.include('userID');
 
  getNameQuery.equalTo("userID", userId);
 
//console.log("request.params " + JSON.stringify(request.params));
//console.log("request.params.objectId " + JSON.stringify(request.params.objectId));
//console.log("json userid " + JSON.stringify(request.params));
//console.log("json userid.id " + JSON.stringify(userId.id));
 
  var userName;
 
  getNameQuery.first({
    success: function(object){
 
      if(typeof(object) == 'undefined'){
          //response.error("No user's name found");
         // console.log("User name undefined");  
          response.error("User name undefined");
      }else{
          userName=object.get('Name');
          //response.success(userName); //tag's userId
          //console.log("Result 1 " + JSON.stringify(object.get('Name')));  //magic
          //console.log("Result 2 " + JSON.stringify(userName));  //magic
          //console.log("userName " + userName);  //magic
 
          response.success(userName);   //tag's userId
          //return userName;
          //callback(userName);
 
          //--------
 
          //--------
      }
 
    },
    error: function(error){
      //response.error("Parse error");
      response.error("get_userName_from_userId Public Function error:"+error);
      //console.log("get_userName_from_userId error");  //magic
      //console.log("error" +error);  //magic
    }
 
    });
});
 
//--------------------------------------------------------------------------------
*/
 
Parse.Cloud.define("tag", function(request, response) {
    tag = request.params.TAG;
 
var query = new Parse.Query(Parse.Installation);
query.equalTo('deviceType', "android");
 
            Parse.Push.send({
             where: query,
              data: {
                alert: "Tag Scanned: " + tag,
                title: "Parnet"
              }
            }, {
              success: function() {
                // Push was successful
              },
              error: function(error) {
                // Handle error
              }
            });
 
 
 
    //data = "{\"TAG\":\"" + tag + "\"}"
    //parseSendRequest(client, "POST", "/1/classes/NFC", date, NULL);
 
 
        var GameScore = Parse.Object.extend("NFC");
        var gameScore = new GameScore();
 
        gameScore.set("TAG", tag);
        //gameScore.set("playerName", "Sean Plott");
        //gameScore.set("cheatMode", false);
 
        gameScore.save(null, {
          success: function(gameScore) {
            // Execute any logic that should take place after the object is saved.
            alert('New object created with objectId: ' + gameScore.id);
          },
          error: function(gameScore, error) {
            // Execute any logic that should take place if the save fails.
            // error is a Parse.Error with an error code and message.
            alert('Failed to create new object, with error code: ' + error.message);
          }
        });
 
    response.success(tag);
 
});