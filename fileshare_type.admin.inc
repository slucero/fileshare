<?php

/**
 * @file
 * Fileshare type editing UI.
 */

/**
 * Fileshare type UI controller.
 */
class FileshareTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage fileshare entity types, including adding
      and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Form callback function for the fileshare type editing form.
 */
function fileshare_type_form($form, &$form_state, $fileshare_type, $op = 'edit') {
  
  if ($op == 'clone') {
    $fileshare_type->label .= ' (cloned)';
    $fileshare_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $fileshare_type->label,
    '#description' => t('The human-readable name of this fileshare type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($fileshare_type->type) ? $fileshare_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'fileshare_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this fileshare type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save fileshare type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the fileshare type form.
 */
function fileshare_type_form_submit(&$form, &$form_state) {
  $fileshare_type = entity_ui_form_submit_build_entity($form, $form_state);
  $fileshare_type->save();
  $form_state['redirect'] = 'admin/structure/fileshare_types';
}

/**
 * Form API submit callback for the fileshare type delete button.
 */
function fileshare_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/fileshare_types/manage/' . $form_state['fileshare_type']->type . '/delete';
}
