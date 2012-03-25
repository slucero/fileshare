<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying delete links
 * as fields.
 */

class fileshare_handler_delete_link_field extends fileshare_handler_link_field {
  /**
   * Construct the fileshare_handler_delete_link_filed object.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }

  /**
   * Render the fileshare delete link as a field.
   */
  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy fileshare to check access against
    $dummy_fileshare = (object) array('type' => $type);
    if (!fileshare_access('edit', $dummy_fileshare)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $fileshare_id = $values->{$this->aliases['fileshare_id']};

    return l($text, 'messages/fileshare/' . $fileshare_id . '/delete');
  }
}
