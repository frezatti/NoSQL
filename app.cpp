// Include the neo4j client library header
#include <iterator>
#include <neo4j-client.h>
#include <cerrno>
#include <errno.h>

// Define some constants for the connection parameters
#define NEO4J_HOST "localhost"
#define NEO4J_PORT 7687
#define NEO4J_USER "neo4j"
#define NEO4J_PASS "neo4j"

// Define a macro for checking errors
#define CHECK_ERROR(err) \
    if (err) { \
        neo4j_perror(stderr, errno," ERROR: " #err); \
        exit(EXIT_FAILURE); \
    }
// This function invokes a callback function for each key-value pair in a map
void neo4j_map_foreach(neo4j_value_t map, void (*callback)(const char *, neo4j_value_t, void *), void *data)
{
    // Check if the map is valid
    if (neo4j_type(map) != NEO4J_MAP)
    {
        return;
    }

    // Get the size of the map
    uint64_t n = neo4j_map_size(map);

    // Iterate over the map
    for (uint64_t i = 0; i < n; ++i)
    {
        // Get the key and value at the current index
        neo4j_value key = neo4j_map_kget(map, neo4j_int(i));
        char key_str[64];
        neo4j_tostring(key, key_str,sizeof(key_str));
        neo4j_value_t value = neo4j_map_get(map, key_str);

        // Invoke the callback function with the key, value, and data
        callback(key_str, value, data);
    }
}

void print_property(const char *key, neo4j_value_t value, void *data);

// A function to create a user node
void create_user(neo4j_connection_t *connection, const char *id, const char *name, const char *email, const char *password, const char *bio, const char *photo)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "CREATE (u:User {id: \"%s\", name: \"%s\", email: \"%s\", password: \"%s\", bio: \"%s\", photo: \"%s\", createdAt: datetime(), updatedAt: datetime()})", id, name, email, password, bio, photo);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to create a post node
void create_post(neo4j_connection_t *connection, const char *id, const char *content, const char *photo)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "CREATE (p:Post {id: \"%s\", content: \"%s\", photo: \"%s\", createdAt: datetime(), updatedAt: datetime()})", id, content, photo);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to create a comment node
void create_comment(neo4j_connection_t *connection, const char *id, const char *content)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "CREATE (c:Comment {id: \"%s\", content: \"%s\", createdAt: datetime(), updatedAt: datetime()})", id, content);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to create a group node
void create_group(neo4j_connection_t *connection, const char *id, const char *name, const char *description, const char *photo)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "CREATE (g:Group {id: \"%s\", name: \"%s\", description: \"%s\", photo: \"%s\", createdAt: datetime(), updatedAt: datetime()})", id, name, description, photo);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to print a property of a node or a relationship
void print_property(const char *key, neo4j_value_t value, void *data)
{
    // Convert the value to a string
    char value_str[64];
    neo4j_tostring(value, value_str, sizeof(value_str));

    // Print the key and the value
    printf("%s: %s\n", key, value_str);
}

int main()
{
    // Initialize the neo4j client library
    neo4j_client_init();

    // Create a neo4j config
    neo4j_config_t* config = neo4j_new_config();
    char endpoint[64];
    sprintf(endpoint, "localhost:%d", NEO4J_PORT);


    // Connect to the database
    neo4j_connection_t *connection = neo4j_connect(NEO4J_HOST, config, NEO4J_INSECURE);
    CHECK_ERROR(connection == NULL);

    // Create some nodes and relationships
    create_user(connection, "u1", "Alice", "alice@example.com", "123456", "Hello, world!", "alice.jpg");
    create_user(connection, "u2", "Bob", "bob@example.com", "654321", "Hi, there!", "bob.jpg");
    create_post(connection, "p1", "This is my first post!", "post1.jpg");
    create_post(connection, "p2", "This is my second post!", "post2.jpg");
    create_comment(connection, "c1", "Nice post!");
    create_comment(connection, "c2", "Thanks!");
    create_group(connection, "g1", "NoSQL Lovers", "A group for NoSQL enthusiasts", "group1.jpg");

    // Close the connection and check for errors
    int err = neo4j_close(connection);

    // Clean up the neo4j client library
    neo4j_client_cleanup();

    // Return 0 to indicate success
    return 0;
}
