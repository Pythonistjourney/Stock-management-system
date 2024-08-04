import { Client, Databases, ID, Query } from "appwrite";

const PROJECT_ID = "6679201e0006bcde0edd";
const DATABASE_ID = "667936d1002fd488695e";
const COLLECTION_ID_PENDINGSTOCKS = "6679371f0018801a3619";
const COLLECTION_ID_STOCKS = "66793727002823479096";

const client = new Client()
  .setEndpoint("https://cloud.appwrite.io/v1")
  .setProject(PROJECT_ID);

const databases = new Databases(client);

const args = process.argv.slice(2);
const command = args[0];
console.log(process)
// <=============== FUNCTIONS RELATED TO FETCHING ACCOUNTS =====================>
if (command === "StorePENDINGSTOCKS") {
  const name=args[1];
  const price=args[2];
  const quantity=args[3];
  StorePENDINGSTOCKS(name,price,quantity);
}
async function StorePENDINGSTOCKS(_name,_price,_quantity){
  try{
    let payload={
      name:_name,
      price:_price,
      quantity:_quantity
    }
    let response = await databases.createDocument(
      DATABASE_ID,
      COLLECTION_ID_PENDINGSTOCKS,
      ID.unique(),
      payload,
    )
  }catch(error){
    console.log("Error Occurred")
  }
}
// StorePENDINGSTOCKS("CAR",parseFloat(500),100);

if (command === "StoreSTOCKS") {
  const name=args[1];
  const price=args[2];
  const quantity=args[3];
  StoreSTOCKS(name,price,quantity);
}
async function StoreSTOCKS(_name,_price,_quantity){
  try{
    let payload={
      name:_name,
      price:_price,
      quantity:_quantity
    }
    let response = await databases.createDocument(
      DATABASE_ID,
      COLLECTION_ID_STOCKS,
      ID.unique(),
      payload,
    )
  }catch(error){
    console.log("Error Occurred")
  }
}
// StoreSTOCKS("BC",parseFloat(1000),2990)
async function FetchSTOCKS(){
  try{
    let response = await databases.listDocuments(
      DATABASE_ID,
      COLLECTION_ID_STOCKS,
      []
    )
    response.documents.forEach((doc)=>{
      console.log(`${doc.name} `+`${doc.price} `+`${doc.quantity}`)
    })
  }catch(error){
    console.log("Error Occurred")
  }
}
