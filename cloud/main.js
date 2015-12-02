// Use Parse.Cloud.define to define as many cloud functions as you want.
// For example:
Parse.Cloud.define("hello", function(request, response) {
  response.success("Hello world!");
});
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
Parse.Cloud.define("echo", function(request, response) {
    value = request.params.value;
  response.success(value);
});
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
/*   
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
*/  
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
    
/*   
Parse.Cloud.define("ActiveTest", function(request, response) {
    gateTag = request.params.TAG;   //MCU params are in caps
    gateTime = request.params.TIME; 
    gateToken= request.params.GATE;
    
    // console.log("gateTag: "+gateTag);
    // console.log("gateTime: "+gateTime);
    // console.log("gateToken: "+gateToken);
    
    gateTime_date = new Date(gateTime);
    // console.log("gateTime_date: "+gateTime_date);
    
    var getUserId = Parse.Object.extend("Tag"); //search "Tag" table for the user of received tag
    var getUserIdQuery = new Parse.Query(getUserId);
    
    getUserIdQuery.equalTo("rfidTag", gateTag);
    
    var userID;
    
    //check if tag is registered
    getUserIdQuery.first({
        success: function(object) {
            if (typeof(object) == 'undefined') {
    
                var returnValue={authorization:0, message1:"Error!", message2:"Unregistered Tag"};
                response.success(returnValue);
    
            }else{
                var userID=object.get('userID');   //get the pointer object not the id value
                var tagID=object.id; 
        //--------------------------------------------------------------------------------
    
                var getName = Parse.Object.extend("UserInfo"); //search "Tag" table for the user of received tag
                var getNameQuery = new Parse.Query(getName);
    
                getNameQuery.equalTo("userID", userID);
    
                var userName;
    
                //check if tag is attached to a user
                getNameQuery.first({
                    success: function(object){
    
                    if(typeof(object) == 'undefined'){
    
                        var returnValue={authorization:0, message1:"Error!", message2:"UnregisteredUser"};
                        response.success(returnValue);
    
                    }else{
                    //--------------------------------------------------------------------------------
    
                        userName=object.get('Name') + " " + object.get('LastName');
    
                            //--------------------------------------------------------------------------------
    
                            var checkActive = Parse.Object.extend("ActiveTags"); //search "Tag" table for the user of received tag
                            var checkActiveQuery = new Parse.Query(checkActive);
    
                            var tagIDPointer={"__type":"Pointer","className":"Tag","objectId":tagID};
    
                            checkActiveQuery.equalTo("rfidTag", tagIDPointer);
    
                            //check if tag is active
                            checkActiveQuery.first({
                                success: function(object){
    
                                var ActiveTagObject=object;
    
                                if(typeof(object) == 'undefined'){
                                    //tag not active, create active
                                    //--------------------------------------------------------------------------------
                                        var checkGate = Parse.Object.extend("Gate"); //search "Tag" table for the user of received tag
                                        var checkGateQuery = new Parse.Query(checkGate);
    
                                        checkGateQuery.equalTo("token", gateToken);
    
                                        //check valid gate
                                        checkGateQuery.first({
                                            success: function(object){
    
                                                if(typeof(object) == 'undefined'){
    
                                                    response.error("invalid gateToken");
                                                    //alert.error("invalid gateToken");
    
                                                }else{
    
                                                    //console.log("Valid gateToken: "+object.get("token"));
                                                    var ActiveTag = Parse.Object.extend("ActiveTags");
                                                    var gateType=object.get("gateType");
                                                    //console.log("gateType: "+JSON.stringify(gateType));
    
    
                                                    if(gateType=='exit'){
    
                                                        var returnValue={authorization:0, message1:"Error!", message2:"Exit Only!"};
                                                        response.success(returnValue);
    
                                                    }else if(gateType=='entrance'){
    
                                                        var parkingID=object.get('parkingId');
                                                        //console.log("parkingID: "+JSON.stringify(parkingID));
                                                        //--------------------------------------------------------------------------------
    
                                                        var SaveHistory = Parse.Object.extend("History");
                                                        var saveHistory = new SaveHistory();
    
                                                        saveHistory.set("tagID", tagIDPointer);
                                                        saveHistory.set("parkingID", parkingID);
                                                        saveHistory.set("in", gateTime_date);
                                                        saveHistory.set("paid", false);
    
                                                        saveHistory.save(null, {
                                                            success: function(saveHistory) {
                                                        //--------------------------------------------------------------------------------
    
                                                            var ActiveTag = Parse.Object.extend("ActiveTags");
                                                            var activeTag = new ActiveTag();
    
                                                            activeTag.set("rfidTag", tagIDPointer);
    
                                                            var HistoryPointer = {"__type":"Pointer","className":"History","objectId":saveHistory.id};
                                                            //console.log("HistoryPointer: "+JSON.stringify(HistoryPointer));
                                                            activeTag.set("historyID", HistoryPointer);
    
                                                            activeTag.save(null, {
                                                                success: function(activeTag) {
    
                                                                alert('New ActiveTag created with objectId: ' + activeTag.id);
                                                                alert('New History created with objectId: ' + saveHistory.id);
                                                                var returnValue={authorization:1, message1:"Welcome!", message2:userName};
                                                                response.success(returnValue);
    
                                                                },
                                                                error: function(activeTag, error) {
                                                                    response.error("error saving active tag");
                                                                    alert.error("error saving active tag: "+error);
                                                                }
                                                            });
    
                                                        //--------------------------------------------------------------------------------
    
                                                            },
                                                            error: function(saveHistory, error) {
                                                                response.error("error saving history");
                                                                alert.error("error saving history: "+error);
                                                            }
                                                        });
    
                                                        //--------------------------------------------------------------------------------
                                                    }else{
                                                        response.error("invalid gateType");
                                                    }
                                                }
                                            },
                                            error: function(object) {
                                                response.error("error cheking gate token");
                                                alert.error("error cheking gate token: "+error);
                                            }
                                        });
                                    //--------------------------------------------------------------------------------
                                    }else{
                                    //tag active, delete active
                                    //--------------------------------------------------------------------------------
                                        var checkGate = Parse.Object.extend("Gate"); //search "Tag" table for the user of received tag
                                        var checkGateQuery = new Parse.Query(checkGate);
    
                                        checkGateQuery.equalTo("token", gateToken);
    
                                        //check valid gate
                                        checkGateQuery.first({
                                            success: function(object){
    
                                                if(typeof(object) == 'undefined'){
    
                                                    response.error("invalid gateToken");
                                                    //alert.error("invalid gateToken: "+error);
    
                                                }else{
    
                                                    //console.log("Valid gateToken: "+object.get("token"));
                                                    var ActiveTag = Parse.Object.extend("ActiveTags");
                                                    var gateType=object.get("gateType");
                                                    //console.log("gateType: "+JSON.stringify(gateType));
    
    
                                                    if(gateType=='entrance'){
    
                                                        var returnValue={authorization:0, message1:"Error! Vehicle", message2:"Already inside"};
                                                        response.success(returnValue);
    
                                                    }else if(gateType=='exit'){
    
                                                        var parkingID=object.get('parkingId');
                                                        //console.log("parkingID: "+JSON.stringify(parkingID));
                                                        //--------------------------------------------------------------------------------
    
                                                        var SaveHistory = Parse.Object.extend("History");
                                                        var saveHistory = new SaveHistory();
    
                                                        saveHistory.id = ActiveTagObject.get("historyID").id;
                                                        saveHistory.set("out", gateTime_date);
                                                        saveHistory.set("paid", true);
    
                                                        saveHistory.save(null, {
                                                            success: function(saveHistory) {
                                                        //--------------------------------------------------------------------------------
    
                                                            ActiveTagObject.destroy({
                                                            success: function(ActiveTagObject) {
    
                                                                alert('Old History updated with objectId: ' + ActiveTagObject.get("historyID").id);
                                                                alert('Old ActiveTag deleted with objectId: ' + ActiveTagObject.id);
                                                                var returnValue={authorization:1, message1:"Goodbye!", message2:userName};
                                                                response.success(returnValue);
    
                                                                },
                                                                error: function(ActiveTagObject, error) {
                                                                    response.error("error deleting active tag");
                                                                    alert.error("error deleting active tag: "+error);
                                                              }
                                                            });
    
                                                        //--------------------------------------------------------------------------------
    
                                                            },
                                                            error: function(saveHistory, error) {
                                                                response.error("error saving history");
                                                                alert.error("error saving history: "+error);
                                                            }
                                                        });
    
                                                        //--------------------------------------------------------------------------------
                                                    }else{
                                                        response.error("invalid gateType");
                                                    }
                                                }
                                            },
                                            error: function(object) {
                                                response.error("error cheking gate token");
                                                alert.error("error cheking gate token: "+error);
                                            }
                                        });
                                    //--------------------------------------------------------------------------------
                                    }
                                },
                                error: function(error){
                                    response.error("error checking active tag");
                                    alert.error("error checking active tag: "+error);
                                }
                            });
                    //--------------------------------------------------------------------------------
                         }
                    },
                    error: function(error){
                        response.error("error getting user's Name+LastName");
                        alert.error("error getting user's Name+LastName: "+error);
                    }
                });
        //--------------------------------------------------------------------------------
            }
        },
        error: function(error) {
            response.error("error getting tag's userID");
            alert.error("error getting tag's userID: "+error);
        }
    });
});
*/
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
    
//Parse.Cloud.define("Publish", function(request, response) {
Parse.Cloud.define("ActiveTest", function(request, response) {
    gateTag = request.params.TAG;   //MCU params are in caps
    gateTime = request.params.TIME; 
    gateToken= request.params.GATE;
    
    // console.log("gateTag: "+gateTag);
    // console.log("gateTime: "+gateTime);
    // console.log("gateToken: "+gateToken);
    
    gateTime_date = new Date(gateTime);
    // console.log("gateTime_date: "+gateTime_date);
    
    var getUserId = Parse.Object.extend("Tag"); //search "Tag" table for the user of received tag
    var getUserIdQuery = new Parse.Query(getUserId);
    
    getUserIdQuery.equalTo("rfidTag", gateTag);
    
    var userID;
    
    //check if tag is registered
    getUserIdQuery.first({
        success: function(object) {
            if (typeof(object) == 'undefined') {
    
                var returnValue={authorization:0, message1:"Error!", message2:"Unregistered Tag"};
                response.success(returnValue);
    
            }else{
                var userID=object.get('userID');   //get the pointer object not the id value
                var tagID=object.id; 
        //--------------------------------------------------------------------------------
    
                var getName = Parse.Object.extend("UserInfo"); //search "Tag" table for the user of received tag
                var getNameQuery = new Parse.Query(getName);
    
                getNameQuery.equalTo("userID", userID);
    
                var userName;
    
                //check if tag is attached to a user
                getNameQuery.first({
                    success: function(object){
    
                    if(typeof(object) == 'undefined'){
    
                        var returnValue={authorization:0, message1:"Error!", message2:"UnregisteredUser"};
                        response.success(returnValue);
    
                    }else{
                    //--------------------------------------------------------------------------------
    
                        //userName=object.get('Name') + " " + object.get('LastName');
                        userName=object.get('Name');
    
                            //--------------------------------------------------------------------------------
    
                            var checkActive = Parse.Object.extend("ActiveTags"); //search "Tag" table for the user of received tag
                            var checkActiveQuery = new Parse.Query(checkActive);
    
                            var tagIDPointer={"__type":"Pointer","className":"Tag","objectId":tagID};
    
                            checkActiveQuery.equalTo("rfidTag", tagIDPointer);
    
                            //check if tag is active
                            checkActiveQuery.first({
                                success: function(object){
    
                                var ActiveTagObject=object;
    
                                if(typeof(object) == 'undefined'){
                                    //tag not active, create active
                                    //--------------------------------------------------------------------------------
                                        var checkGate = Parse.Object.extend("Gate"); //search "Tag" table for the user of received tag
                                        var checkGateQuery = new Parse.Query(checkGate);
    
                                        checkGateQuery.equalTo("token", gateToken);
    
                                        //check valid gate
                                        checkGateQuery.first({
                                            success: function(object){
    
                                                if(typeof(object) == 'undefined'){
    
                                                    response.error("invalid gateToken");
                                                    //alert.error("invalid gateToken");
    
                                                }else{
    
                                                    //console.log("Valid gateToken: "+object.get("token"));
                                                    var ActiveTag = Parse.Object.extend("ActiveTags");
                                                    var gateType=object.get("gateType");
                                                    //console.log("gateType: "+JSON.stringify(gateType));
    
    
                                                    if(gateType=='exit'){
    
                                                        var returnValue={authorization:0, message1:"Error!", message2:"Exit Only!"};
                                                        response.success(returnValue);
    
                                                    }else if(gateType=='entrance'){
    
                                                        var parkingID=object.get('parkingId');
                                                        //console.log("parkingID: "+JSON.stringify(parkingID));
                                                        //--------------------------------------------------------------------------------
    
                                                        var SaveHistory = Parse.Object.extend("History");
                                                        var saveHistory = new SaveHistory();
    
                                                        saveHistory.set("tagID", tagIDPointer);
                                                        saveHistory.set("parkingID", parkingID);
                                                        saveHistory.set("in", gateTime_date);
                                                        saveHistory.set("paid", false);
    
                                                        saveHistory.save(null, {
                                                            success: function(saveHistory) {
                                                        //--------------------------------------------------------------------------------
    
                                                            var ActiveTag = Parse.Object.extend("ActiveTags");
                                                            var activeTag = new ActiveTag();
    
                                                            activeTag.set("rfidTag", tagIDPointer);
    
                                                            var HistoryPointer = {"__type":"Pointer","className":"History","objectId":saveHistory.id};
                                                            //console.log("HistoryPointer: "+JSON.stringify(HistoryPointer));
                                                            activeTag.set("historyID", HistoryPointer);
    
                                                            activeTag.save(null, {
                                                                success: function(activeTag) {
    
                                                                alert('New ActiveTag created with objectId: ' + activeTag.id);
                                                                var newActiveTag=activeTag;
                                                                alert('New History created with objectId: ' + saveHistory.id);
                                                                var newHistory=saveHistory;
                                                                // var returnValue={authorization:1, message1:"Welcome!", message2:userName};
                                                                // response.success(returnValue);
 
                                                                //--------------------------------------------------------------------------------
 
                                                                    var GetWalletId = Parse.Object.extend("Wallet"); //search "Tag" table for the user of received tag
                                                                    var GetWalletIdQuery = new Parse.Query(GetWalletId);
 
                                                                    var userIDPointer={"__type":"Pointer","className":"_User","objectId":userID.id}; 
                                                                                                                                                                                                                                    
                                                                    GetWalletIdQuery.equalTo("userID", userIDPointer);
                                                                                                                                                                                                                                                                                                                  
                                                                    GetWalletIdQuery.first({
                                                                        success: function(object){
                                                                    
                                                                            if(typeof(object) == 'undefined'){
                                                                    
                                                                                response.error("GetWalletIdQuery undefined");                                                                                                
 
                                                                                }else{
 
                                                                                    var walletID=object.id;
                                                                                    var balance=object.get("credits");
                                                                                    console.log("Wallet balance: "+JSON.stringify(balance));
 
                                                                                    //--------------------------------------------------------------------------------
                                                                                        var GetRate = Parse.Object.extend("Rate"); //search "Tag" table for the user of received tag
                                                                                        var GetRateQuery = new Parse.Query(GetRate);
 
                                                                                        console.log("ParkingID: "+JSON.stringify(parkingID));   
                                                                                        var parkingIDPointer={"__type":"Pointer","className":"Parking","objectId":parkingID.id}; 
                                                                                        console.log("ParkingID pointer: "+JSON.stringify(parkingIDPointer));    
                                                                                                                                                                                                                                                                                                                                                                
                                                                                        GetRateQuery.equalTo("parkingID", parkingIDPointer);
                                                                                                                                                                                                                                                                                                                                                 
                                                                                        GetRateQuery.first({
                                                                                            success: function(object){
                                                                                        
                                                                                                if(typeof(object) == 'undefined'){
                                                                                        
                                                                                                    response.error("GetRateQuery undefined");                                                                                                
 
                                                                                                    }else{
 
                                                                                                        var mainRate=object.get("main");
                                                                                                        console.log("mainRate: "+JSON.stringify(mainRate)); 
 
                                                                                                        if(mainRate<=balance){
                                                                                                            var returnValue={authorization:1, message1:"Welcome "+userName, message2:"Balance: $"+balance};
                                                                                                            response.success(returnValue);
                                                                                                        }else{
                                                                                                            console.log("ActiveTagObject: "+JSON.stringify(newActiveTag));
                                                                                                            //--------------------------------------------------------------------------------
                                                                                                            newActiveTag.destroy({
                                                                                                                success: function(ActiveTagObject) {
 
                                                                                                                //--------------------------------------------------------------------------------
                                                                                                                    console.log("newHistory: "+JSON.stringify(newHistory));
                                                                                                                    newHistory.destroy({
                                                                                                                        success: function(ActiveTagObject) {
 
                                                                                                                            var returnValue={authorization:0, message1:"Insufficient", message2:"Funds $"+balance};
                                                                                                                            response.success(returnValue);
                                                                                                                     },
                                                                                                                        error: function(ActiveTagObject, error) {
                                                                                                                            response.error("error deleting active tag");
                                                                                                                            alert.error("error deleting active tag: "+error);
                                                                                                                      }
                                                                                                                    });
                                                                                                                //--------------------------------------------------------------------------------
                                                                                                             },
                                                                                                                error: function(ActiveTagObject, error) {
                                                                                                                    response.error("error deleting active tag");
                                                                                                                    alert.error("error deleting active tag: "+error);
                                                                                                              }
                                                                                                            });
 
                                                                                                        //--------------------------------------------------------------------------------
 
                                                                                                        }
                                                                                                    //--------------------------------------------------------------------------------
                                                                                                    }
                                                                                                },
                                                                                            error: function(error) {
                                                                                                response.error("GetRateQuery error");
                                                                                                //alert.error("GetRateQuery error: "+error);
                                                                                          }
                                                                                        });
                                                                                 //--------------------------------------------------------------------------------
                                                                                }
                                                                            },
                                                                        error: function(error) {
                                                                            response.error("GetWalletIdQuery error");
                                                                            alert.error("GetWalletIdQuery error: "+error);
                                                                      }
                                                                    }); 
                                                                //--------------------------------------------------------------------------------
                                                                },
                                                                error: function(activeTag, error) {
                                                                    response.error("error saving active tag");
                                                                    alert.error("error saving active tag: "+error);
                                                                }
                                                            });
                                                        //--------------------------------------------------------------------------------
    
                                                            },
                                                            error: function(saveHistory, error) {
                                                                response.error("error saving history");
                                                                alert.error("error saving history: "+error);
                                                            }
                                                        });
                                                        //--------------------------------------------------------------------------------
                                                    }else{
                                                        response.error("invalid gateType");
                                                    }
                                                }
                                            },
                                            error: function(object) {
                                                response.error("error cheking gate token");
                                                alert.error("error cheking gate token: "+error);
                                            }
                                        });
                                    //--------------------------------------------------------------------------------
                                    }else{
                                    //tag active, delete active
                                    //--------------------------------------------------------------------------------
                                        var checkGate = Parse.Object.extend("Gate"); //search "Tag" table for the user of received tag
                                        var checkGateQuery = new Parse.Query(checkGate);
    
                                        checkGateQuery.equalTo("token", gateToken);
    
                                        //check valid gate
                                        checkGateQuery.first({
                                            success: function(object){
    
                                                if(typeof(object) == 'undefined'){
    
                                                    response.error("invalid gateToken");
                                                    //alert.error("invalid gateToken: "+error);
    
                                                }else{
    
                                                    //console.log("Valid gateToken: "+object.get("token"));
                                                    var ActiveTag = Parse.Object.extend("ActiveTags");
                                                    var gateType=object.get("gateType");
                                                    //console.log("gateType: "+JSON.stringify(gateType));
    
    
                                                    if(gateType=='entrance'){
    
                                                        var returnValue={authorization:0, message1:"Error! Vehicle", message2:"Already inside"};
                                                        response.success(returnValue);
    
                                                    }else if(gateType=='exit'){
    
                                                        var parkingID=object.get('parkingId');
                                                        //console.log("parkingID: "+JSON.stringify(parkingID));
                                                        var ActiveInDate;
                                                        //--------------------------------------------------------------------------------
  
                                                            var GetHistoryInDate = Parse.Object.extend("History"); //search "Tag" table for the user of received tag
                                                            var GetHistoryInDateQuery = new Parse.Query(GetHistoryInDate);
                                                            
                                                            //thePointer={"__type":"Pointer","className":"Tag","objectId":pointerId};
                                                            
                                                            GetHistoryInDateQuery.equalTo("objectId", ActiveTagObject.get("historyID").id);
                                                            console.log("ActiveTag objectId: "+ActiveTagObject.get("historyID").id);
                                                            
                                                            GetHistoryInDateQuery.first({
                                                                success: function(object){
                                                            
                                                                    if(typeof(object) == 'undefined'){
                                                            
                                                                        response.error("GetHistoryInDateQuery undefined");
                                                            
                                                                    }else{
                                                            
                                                                        //response.success(object);   //tag's userId
                                                                        ActiveInDate = object.get("in");
                                                                        console.log("ActiveInDate " + JSON.stringify(object.get("in"))); 
  
                                                                        //--------------------------------------------------------------------------------
                                                                        var SaveHistory = Parse.Object.extend("History");
                                                                        var saveHistory = new SaveHistory();
                    
                                                                        saveHistory.id = ActiveTagObject.get("historyID").id;
                                                                        saveHistory.set("out", gateTime_date);
                                                                        saveHistory.set("paid", true);
  
                                                                        console.log("In date: "+ActiveInDate);
                                                                  
                                                                        //--------------------------------------------------------------------------------
                                                                        var startDate = new Date(ActiveInDate);
                                                                        var endDate = new Date(gateTime_date);
                                                                        var starthour = parseInt(startDate.getHours());
                                                                        var endhour = parseInt(endDate.getHours());
                                                                        var hoursDiff;
  
                                                                        if(starthour>endhour){
                                                                            //alert('Hours diff:' + parseInt(starthour-endhour));
                                                                            hoursDiff=starthour-endhour;
                                                                        }
                                                                        else{
                                                                            //alert('Hours diff:' + parseInt(endhour-starthour));
                                                                            hoursDiff=endhour-starthour;
                                                                        }
 
                                                                        console.log("hoursDiff: "+hoursDiff);
                                                                        //--------------------------------------------------------------------------------
  
                                                                        saveHistory.save(null, {
                                                                            success: function(saveHistory) {
                                                                        //--------------------------------------------------------------------------------
                    
                                                                            ActiveTagObject.destroy({
                                                                            success: function(ActiveTagObject) {
                    
                                                                                alert('Old History updated with objectId: ' + ActiveTagObject.get("historyID").id);
                                                                                alert('Old ActiveTag deleted with objectId: ' + ActiveTagObject.id);
 
                                                                                    //--------------------------------------------------------------------------------
                                                                                    var GetWalletId = Parse.Object.extend("Wallet"); //search "Tag" table for the user of received tag
                                                                                    var GetWalletIdQuery = new Parse.Query(GetWalletId);
 
                                                                                    var userIDPointer={"__type":"Pointer","className":"_User","objectId":userID.id}; 
                                                                                                                                                                                                                                                    
                                                                                    GetWalletIdQuery.equalTo("userID", userIDPointer);
                                                                                                                                                                                                                                                                                                                                  
                                                                                    GetWalletIdQuery.first({
                                                                                        success: function(object){
                                                                                    
                                                                                            if(typeof(object) == 'undefined'){
                                                                                    
                                                                                                response.error("GetWalletIdQuery undefined");                                                                                                
 
                                                                                                }else{
 
                                                                                                    var walletID=object.id;
                                                                                                    var balance=object.get("credits");
                                                                                                    console.log("Wallet balance: "+JSON.stringify(balance));
 
                                                                                                    // var returnValue={authorization:1, message1:"Goodbye!", message2:userName};
                                                                                                    // response.success(returnValue);
 
                                                                                                    //--------------------------------------------------------------------------------
                                                                                                        var GetRate = Parse.Object.extend("Rate"); //search "Tag" table for the user of received tag
                                                                                                        var GetRateQuery = new Parse.Query(GetRate);
 
                                                                                                        console.log("ParkingID: "+JSON.stringify(parkingID));   
                                                                                                        var parkingIDPointer={"__type":"Pointer","className":"Parking","objectId":parkingID.id}; 
                                                                                                        console.log("ParkingID pointer: "+JSON.stringify(parkingIDPointer));    
                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                        GetRateQuery.equalTo("parkingID", parkingIDPointer);
                                                                                                                                                                                                                                                                                                                                                                 
                                                                                                        GetRateQuery.first({
                                                                                                            success: function(object){
                                                                                                        
                                                                                                                if(typeof(object) == 'undefined'){
                                                                                                        
                                                                                                                    response.error("GetRateQuery undefined");                                                                                                
 
                                                                                                                    }else{
 
                                                                                                                        var mainRate=object.get("main");
                                                                                                                        var perHourRate=object.get("perHour");
 
                                                                                                                        //--------------------------------------------------------------------------------
 
                                                                                                                            var Transaction = Parse.Object.extend("Transaction");
                                                                                                                            var TransactionQuery = new Transaction();
                                                                    
                                                                                                                            TransactionQuery.set("rfidTag", tagIDPointer);
                                                                    
                                                                                                                            var walletIDPointer = {"__type":"Pointer","className":"Wallet","objectId":walletID};
                                                                                                                            console.log("walletIDPointer: "+JSON.stringify(walletIDPointer));
 
                                                                                                                            TransactionQuery.set("walletID", walletIDPointer);
                                                                                                                            TransactionQuery.set("credit", 0);
 
                                                                                                                            var debit=mainRate+(perHourRate*hoursDiff);
                                                                                                                            TransactionQuery.set("debit", debit);
                                                                                                                            console.log("debit: "+debit);
 
                                                                                                                            TransactionQuery.set("balance", balance-debit);
                                                                                                                            console.log("new balance: "+(balance-debit));
                                                                    
                                                                                                                            TransactionQuery.save(null, {
                                                                                                                                success: function(TransactionQuery) {
 
                                                                                                                                //--------------------------------------------------------------------------------
 
                                                                                                                                    var updateWallet = Parse.Object.extend("Wallet");
                                                                                                                                    var updateWalletQuery = new updateWallet();
 
                                                                                                                                    updateWalletQuery.id = walletID;
                                                                                                                                    updateWalletQuery.set("credits", balance-debit);                                                                        
                                                                            
                                                                                                                                    updateWalletQuery.save(null, {
                                                                                                                                        success: function(updateWalletQuery) {
                                                                            
                                                                                                                                        var returnValue={authorization:1, message1:"Bye "+userName+"!", message2:"$"+debit+" Balance: $"+(balance-debit)};
                                                                                                                                        response.success(returnValue);
                                                                         
                                                                                                                                        },
                                                                                                                                        error: function(updateWalletQuery, error) {
                                                                                                                                            response.error("error saving wallet");
                                                                                                                                            //alert.error("error saving active tag: "+error);
                                                                                                                                        }
                                                                                                                                    });
                                                                                                                                //--------------------------------------------------------------------------------                                                                   
                                                                                                                                },
                                                                                                                                error: function(TransactionQuery, error) {
                                                                                                                                    response.error("error saving transaction");
                                                                                                                                    //alert.error("error saving active tag: "+error);
                                                                                                                                }
                                                                                                                            });
                                                                                                                    //--------------------------------------------------------------------------------
                                                                                                                    }
                                                                                                                },
                                                                                                            error: function(error) {
                                                                                                                response.error("GetRateQuery error");
                                                                                                                //alert.error("GetRateQuery error: "+error);
                                                                                                          }
                                                                                                        });
                                                                                                 //--------------------------------------------------------------------------------
                                                                                                }
                                                                                            },
                                                                                        error: function(error) {
                                                                                            response.error("GetWalletIdQuery error");
                                                                                            alert.error("GetWalletIdQuery error: "+error);
                                                                                      }
                                                                                    }); 
                                                                                    //--------------------------------------------------------------------------------                  
                                                                                },
                                                                                error: function(ActiveTagObject, error) {
                                                                                    response.error("error deleting active tag");
                                                                                    alert.error("error deleting active tag: "+error);
                                                                              }
                                                                            });                  
                                                                        //--------------------------------------------------------------------------------            
                                                                            },
                                                                            error: function(saveHistory, error) {
                                                                                response.error("error saving history");
                                                                                alert.error("error saving history: "+error);
                                                                            }
                                                                        });                  
                                                                //--------------------------------------------------------------------------------
                                                                    }
                                                            
                                                                },
                                                                error: function(error){
                                                                    response.error("get_userId_from_tag Public Function error:"+error);
                                                            
                                                                }
                                                            
                                                            });
                                                    //--------------------------------------------------------------------------------
                                                    }else{
                                                        response.error("invalid gateType");
                                                    }
                                                }
                                            },
                                            error: function(error) {
                                                response.error("error cheking gate token");
                                                alert.error("error cheking gate token: "+error);
                                            }
                                        });
                                    //--------------------------------------------------------------------------------
                                    }
                                },
                                error: function(error){
                                    response.error("error checking active tag");
                                    alert.error("error checking active tag: "+error);
                                }
                            });
                    //--------------------------------------------------------------------------------
                         }
                    },
                    error: function(error){
                        response.error("error getting user's Name+LastName");
                        alert.error("error getting user's Name+LastName: "+error);
                    }
                });
        //--------------------------------------------------------------------------------
            }
        },
        error: function(error) {
            response.error("error getting tag's userID");
            alert.error("error getting tag's userID: "+error);
        }
    });
});
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
/*
Parse.Cloud.define("poninterTest", function(request, response) {
    pointerId = request.params.POINTER; //MCU params are in caps
    
    var poninterTest = Parse.Object.extend("ActiveTags"); //search "Tag" table for the user of received tag
    var poninterTestQuery = new Parse.Query(poninterTest);
    
    thePointer={"__type":"Pointer","className":"Tag","objectId":pointerId};
    
    poninterTestQuery.equalTo("rfidTag", thePointer);
    
    
    poninterTestQuery.first({
        success: function(object){
    
            if(typeof(object) == 'undefined'){
    
                response.error("undefined");
    
            }else{
    
                response.success(object);   //tag's userId
                console.log("object " + JSON.stringify(object));  //magic
            }
    
        },
        error: function(error){
            response.error("get_userId_from_tag Public Function error:"+error);
    
        }
    
    });
    
});
 
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
///*
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
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
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
//*/
    
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
    
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