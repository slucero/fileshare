<?php
/**
 * @file
 * Contains a Views argument handler to take care of filter fileshares based
 * on parties involved.
 */

class fileshare_handler_parties_involved_argument extends views_handler_argument {
  /**
   * Constructs the argument for use by Views.
   */
  function construct() {
    parent::construct();
  }

  /**
   * Allow the argument form to be exposed.
   */
  function can_expose() {
    return TRUE;
  }

  /**
   * Ensure table presence in query and alter the query as necessary for search.
   */
  function query() {
    $this->ensure_my_table();

    if (!empty($this->argument)) {
      // Add filter for the argument value
      $this->query->add_where('parties_involved', "$this->table_alias.sender", $this->argument);
      $this->query->add_where('parties_involved', "$this->table_alias.recipient", $this->argument);
      $this->query->set_where_group('OR', 'parties_involved');
    }
  }
}
