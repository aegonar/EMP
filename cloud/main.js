
Parse.Cloud.define("get_username_from_tag", function(request, response) {
	tag = request.params.TAG;	//MCU params are in caps

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
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

Parse.Cloud.define("ActiveTest", function(request, response) {
	tag = request.params.TAG;	//MCU params are in caps

	var getUserId = Parse.Object.extend("Tag"); //search "Tag" table for the user of received tag
	var getUserIdQuery = new Parse.Query(getUserId);

	getUserIdQuery.equalTo("rfidTag", tag);

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

							    if(typeof(object) == 'undefined'){
							    	//tag not active, create active
							    	//TODO check gate
									//--------------------------------------------------------------------------------
										var ActiveTag = Parse.Object.extend("ActiveTags");
										var activeTag = new ActiveTag();

										activeTag.set("rfidTag", tagIDPointer);

										activeTag.save(null, {
											success: function(activeTag) {
												alert('New ActiveTag created with objectId: ' + activeTag.id);
												var returnValue={authorization:1, message1:"Welcome!", message2:userName};
												response.success(returnValue);
											},
											error: function(activeTag, error) {
											   	response.error("error saving active tag");
											   	alert.error("error saving active tag: "+error);
											}
										});
									//--------------------------------------------------------------------------------
							  		}else{
									//tag active, delete active
									//TODO check gate
									//--------------------------------------------------------------------------------							
										object.destroy({
										  	success: function(object) {
										  		alert('Old ActiveTag deleted with objectId: ' + object.id);
												var returnValue={authorization:1, message1:"Goodbye!", message2:userName};
												response.success(returnValue);
										 	 },
										 	 error: function(object, error) {
										   		response.error("error deleting active tag");
										   		alert.error("error deleting active tag: "+error);
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