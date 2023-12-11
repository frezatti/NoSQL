// Include the neo4j client library header
#include <neo4j-client.h>

// Define some constants for the connection parameters
#define NEO4J_HOST "localhost"
#define NEO4J_PORT 7687
#define NEO4J_USER "neo4j"
#define NEO4J_PASS "neo4j"

// Define a macro for checking errors
#define CHECK_ERROR(err) \
    if (err) { \
        neo4j_perror(stderr, errno, err); \
        exit(EXIT_FAILURE); \
    }

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

// A function to create a message node
void create_message(neo4j_connection_t *connection, const char *id, const char *content)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "CREATE (m:Message {id: \"%s\", content: \"%s\", createdAt: datetime(), updatedAt: datetime()})", id, content);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to create a relationship between two nodes
void create_relationship(neo4j_connection_t *connection, const char *type, const char *label1, const char *id1, const char *label2, const char *id2)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "MATCH (n1:%s {id: \"%s\"}), (n2:%s {id: \"%s\"}) CREATE (n1)-[:%s]->(n2)", label1, id1, label2, id2, type);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to query nodes by a label and an id
void query_node(neo4j_connection_t *connection, const char *label, const char *id)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "MATCH (n:%s {id: \"%s\"}) RETURN n", label, id);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Fetch the first result and check for errors
    neo4j_result_t *result = neo4j_fetch_next(results);
    CHECK_ERROR(result == NULL);

    // Get the node value and check for errors
    neo4j_value_t node = neo4j_result_field(result, 0);
    CHECK_ERROR(neo4j_is_null(node));

    // Print the node properties
    printf("Node %s with id %s has the following properties:\n", label, id);
    neo4j_map_foreach(neo4j_node_properties(node), print_property, NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to query relationships by a type and two ids
void query_relationship(neo4j_connection_t *connection, const char *type, const char *id1, const char *id2)
{
    // Construct the cypher query
    char query[256];
    sprintf(query, "MATCH (n1)-[r:%s]->(n2) WHERE n1.id = \"%s\" AND n2.id = \"%s\" RETURN r", type, id1, id2);

    // Execute the query and check for errors
    neo4j_result_stream_t *results = neo4j_run(connection, query, neo4j_null);
    CHECK_ERROR(results == NULL);

    // Fetch the first result and check for errors
    neo4j_result_t *result = neo4j_fetch_next(results);
    CHECK_ERROR(result == NULL);

    // Get the relationship value and check for errors
    neo4j_value_t rel = neo4j_result_field(result, 0);
    CHECK_ERROR(neo4j_is_null(rel));

    // Print the relationship properties
    printf("Relationship %s between nodes with ids %s and %s has the following properties:\n", type, id1, id2);
    neo4j_map_foreach(neo4j_relationship_properties(rel), print_property, NULL);

    // Discard the results and check for errors
    int err = neo4j_close_results(results);
    CHECK_ERROR(err != 0);
}

// A function to print a property key and value
int print_property(const char *key, neo4j_value_t value, void *data)
{
    // Convert the value to a string and check for errors
    char buf[256];
    int n = neo4j_tostring(value, buf, sizeof(buf));
    CHECK_ERROR(n < 0);

    // Print the key and value
    printf("%s: %s\n", key, buf);

    // Return 0 to indicate success
    return 0;
}

// The main function
int main()
{
    // Initialize the neo4j client library
    neo4j_client_init();

    // Connect to the neo4j database and check for errors
    neo4j_connection_t *connection = neo4j_connect(NEO4J_HOST, NEO4J_PORT, NEO4J_USER, NEO4J_PASS, NEO4J_INSECURE);
    CHECK_ERROR(connection == NULL);

    // Create some nodes and relationships
    create_user(connection, "u1", "Alice", "alice@example.com", "123456", "Hello, world!", "alice.jpg");
    create_user(connection, "u2", "Bob", "bob@example.com", "654321", "I like graphs!");
    create_post(connection, "p1", "This is my first post!", "post1.jpg");
    create_post(connection, "p2", "This is my second post!", "post2.jpg");
    create_comment(connection, "c1", "Nice post!");
    create_comment(connection, "c2", "Thanks!");
    create_group(connection, "g1", "NoSQL Lovers", "A group for NoSQL enthusiasts", "group1.jpg");
    create_message(connection, "m1", "Hi, Bob!");
    create_message(connection, "m2", "Hi, Alice!");
    create_relationship(connection, "CREATES", "User", "u1", "Post", "p1");
    create_relationship(connection, "CREATES", "User", "u1", "Post", "p2");
    create_relationship(connection, "CREATES", "User", "u1", "Comment", "c2");
    create_relationship(connection, "CREATES", "User", "u2", "Comment", "c1");
    create_relationship(connection, "HAS", "Post", "p1", "Comment", "c1");
    create_relationship(connection, "HAS", "Post", "p2", "Comment", "c2");
    create_relationship(connection, "JOINS", "User", "u1", "Group", "g1");
    create_relationship(connection, "JOINS", "User", "u2", "Group", "g1");
    create_relationship(connection, "ADMINISTERS", "User", "u1", "Group", "g1");
    create_relationship(connection, "SENDS", "User", "u1", "Message", "m1");
    create_relationship(connection, "SENDS", "User", "u2", "Message", "m2");
    create_relationship(connection, "RECEIVES", "User", "u2", "Message", "m1");
    create_relationship(connection, "RECEIVES", "User", "u1", "Message", "m2");

    // Query some nodes and relationships
    query_node(connection, "User", "u1");
    query_node(connection, "Post", "p1");
    query_node(connection, "Comment", "c1");
    query_node(connection, "Group", "g1");
    query_node(connection, "Message", "m1");
    query_relationship(connection, "CREATES", "u1", "p1");
    query_relationship(connection, "HAS", "p1", "c1");
    query_relationship(connection, "JOINS", "u1", "g1");
    query_relationship(connection, "SENDS", "u1", "m1");
    query_relationship(connection, "RECEIVES", "u2", "m1");

    // Close the connection and check for errors
    int err = neo4j_close(connection);
    CHECK_ERROR(err != 0);

    // Clean up the neo4j client library
    neo4j_client_cleanup();

    // Return 0 to indicate success
    return 0;
}
