<?php

/**
 * @file
 * Contains a Views argument handler to take care of filter fileshares based
 * on recipients and carbon copied parties.
 */

class fileshare_handler_inbox_argument extends views_handler_argument {
  /**
   * @todo Refactor cc storage such that it is stored in it's own column instead of using a LIKE on the data column.
   */
  function query() {
    $account = user_load($this->argument);

    $this->ensure_my_table();

    if (!empty($this->argument)) {
      $this->query->add_where('inbox_filter', "$this->table_alias.recipient", $this->argument);
			$this->query->add_where_expression('inbox_filter', "LOWER(CONVERT($this->table_alias.data USING latin1)) LIKE :email", array(':email' => '%' . strtolower($account->mail) . '%'));
      $this->query->set_where_group('OR', 'inbox_filter');
    }
  }
}
