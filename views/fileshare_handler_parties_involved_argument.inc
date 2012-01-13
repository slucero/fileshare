<?php

/**
 * @file
 * Contains a Views argument handler to take care of filter fileshares based
 * on parties involved.
 */

class fileshare_handler_parties_involved_argument extends views_handler_argument {
  function construct() {
    parent::construct();
  }

  function can_expose() {
    return TRUE;
  }

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
