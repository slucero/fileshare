<?php
/**
 * @file
 * A views handler to provide general interactive operations with a fileshare as a field.
 */

/**
 * This field handler aggregates operations that can be done on a fileshare
 * under a single field providing a more flexible way to present them in a view
 */
class fileshare_handler_fileshare_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['fileshare_id'] = 'fileshare_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('fileshare', 'messages/message', array($values->{$this->aliases['fileshare_id']}));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
