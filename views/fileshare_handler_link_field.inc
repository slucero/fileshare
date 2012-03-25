<?php
/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class fileshare_handler_link_field extends views_handler_field {
  /**
   * Constructs the fileshare link field.
   */
  function construct() {
    parent::construct();

    $this->additional_fields['fileshare_id'] = 'fileshare_id';
    $this->additional_fields['type'] = 'type';
  }

  /**
   * Provide a default value for the additional fields added to the options
   * form.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Modify the field options form to provide an option for text to display.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  /**
   * Ensure tables and fields are present in query.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Render the fileshare link as a Views field.
   */
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $fileshare_id = $values->{$this->aliases['fileshare_id']};

    return l($text, 'messages/message/' . $fileshare_id);
  }
}
