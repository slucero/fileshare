<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class fileshare_handler_subject_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['fileshare_id'] = 'fileshare_id';
    $this->additional_fields['subject'] = 'subject';
    $this->additional_fields['type'] = 'type';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link'] = array('default' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display as link to fileshare.'),
      '#default_value' => $this->options['link'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $fileshare_id = $values->{$this->aliases['fileshare_id']};
    $subject = $values->{$this->aliases['subject']};

    if ($this->options['link']) {
      $subject = l($subject, 'messages/message/' . $fileshare_id);
    }

    return $subject;
  }
}
