// Import required modules
let express = require('express');
let path = require('path');
let fs = require('fs');
let MongoClient = require('mongodb').MongoClient;
let bodyParser = require('body-parser');
const cors = require('cors');

let app = express();
app.use(cors());  // Enable CORS for all domains

// Middleware to parse URL-encoded and JSON request bodies
// if i receive data from a form like this: name=John&age=30
// i get { "name": "John", "age": "30" }
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json()); // parse body into javascript from json

// Serve the main HTML file
//app.get: Defines a route that handles GET requests to a specified path.
//'/': The root URL of the server. For example, http://localhost:3000/.
//Callback Function (req, res): The function to execute when the route is accessed.
    //req: Represents the request object (information from the client).
    //res: Represents the response object (used to send data back to the client).  
app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname, "index.html"));
});

// Serve a profile picture
app.get('/profile-picture', function (req, res) {
  let img = fs.readFileSync(path.join(__dirname, "images/FOTO.jpg")); // Reads the file synchronously from the file system.
  res.writeHead(200, { 'Content-Type': 'image/jpg' }); // Sends a response header with status code 200 (OK).
  res.end(img, 'binary'); // Ends the response and sends the image data to the client.
});

// MongoDB connection URLs for local and Docker environments
let mongoUrlLocal = "mongodb://admin:password@localhost:27017";
let mongoUrlDocker = "mongodb://admin:password@mongodb";

// MongoDB connection options to avoid deprecation warnings
let mongoClientOptions = { useNewUrlParser: true, useUnifiedTopology: true };

// Database name
let databaseName = "user-account";

// API to update a user profile
app.post('/update-profile', async function (req, res) {
  let userObj = req.body; // Request body containing user profile data

  try {
    // Connect to MongoDB
    const client = await MongoClient.connect(mongoUrlLocal, mongoClientOptions);
    const db = client.db(databaseName);
    userObj['userid'] = 1; // Assign a default user ID for this example

    let myquery = { userid: 1 }; // Query to find the user
    let newvalues = { $set: userObj }; // Data to update

    // Update or insert the user profile
    const result = await db.collection("users").updateOne(myquery, newvalues, { upsert: true });


    // Check if the update was successful or if a new document was created
    // If no document was modified, that means it's an insert
    if (result.matchedCount === 0 && result.upsertedCount === 0) {
      return res.status(400).send({ message: "Profile update failed" });
    }

    // Fetch the updated document
    const updatedProfile = await db.collection("users").findOne({ userid: 1 });

    client.close(); // Close the database connection

    // Send the updated profile back as the response
    res.json({
      message: "Profile updated successfully",
      updatedUser: updatedProfile
    });

  } catch (err) {
    console.error("Error updating profile:", err);
    res.status(500).send({ error: "Failed to update profile" });
  }
});


// API to retrieve a user profile
app.get('/get-profile', async function (req, res) {
  try {
    let response = {};

    // Connect to MongoDB
    const client = await MongoClient.connect(mongoUrlLocal, mongoClientOptions);
    const db = client.db(databaseName);
    const myquery = { userid: 1 }; // Query to find the user by ID

    // Fetch the user profile from the database
    const result = await db.collection("users").findOne(myquery);

    if (!result) {
      return res.send({ message: "No profile found" });
    }

    response = result; // Store the result
    client.close(); // Close the database connection

    // Send the profile or an empty object if no profile was found
    res.json(response);

  } catch (err2) {
    console.error("Error fetching profile:", err2);
    res.status(500).send({ error: "Failed to fetch profile" });
  }
});


// Start the Express server
app.listen(3000, function () {
  console.log("app listening on port 3000!");
});
