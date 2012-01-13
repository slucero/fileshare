<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class fileshare_handler_status_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['status'] = 'status';
    $this->additional_fields['date_sent'] = 'date_sent';
    $this->additional_fields['date_read'] = 'date_read';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $read = $values->{$this->aliases['status']};

    if ($read) {
      $status = '<span class="read">' . t('Read') . '</span>';
    } 
    else {
      $status = '<span class="unread">' . t('Unread') . '</span>';
    }

    return $status;
  }
}
