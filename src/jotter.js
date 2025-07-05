const jsonString = '{"Name" : "Stanley", "Age" : 22 }';

try {
    const parseResult = JSON.parse(jsonString)

    // let name = parseResult.name;
    // let age = parseResult.age;

    console.log(`His name is ${parseResult.Name} and he is ${parseResult.Age} years old`);
} catch (error) {
    console.log(error)
}
