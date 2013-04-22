<?php

/**
 * @file
 * Admin callbacks for Islandora Preingest Transform
 */

/**
 * Admin form.
 */
function delft_admin($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $form = array();
  $password = variable_get('delftdora_fedora_password', '');
  $executable_path = variable_get('delftdora_fedora_path', '/usr/local/fedora/server/bin');
  $log_path = variable_get('delftdora_fedora_log_path', '/usr/local/fedora/server/logs');

  $form['delftdora_fedora_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Password'),
    '#description' => t('Password for user fedoraAdmin.'),
    '#default_value' => $password,
  );

  $form['delftdora_fedora_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Executable Path'),
    '#description' => t('Full server Path to fedora-modify script'),
    '#default_value' => $executable_path,
  );
  $form['delftdora_fedora_log_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Logging Path'),
    '#description' => t('Full server path to Fedora logging directory'),
    '#default_value' => $log_path,
  );

  $form['rels_ext_select'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Models to Replace RELS-EXT'),
    '#description' => t('Objects of selected content models will have RELS-EXT replaced by the output of sdef:makeDS'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['rels_ext_select']['cmodels'] = islandora_content_model_select_table_form_element('delft_content_models');
  $form['rels_ext_select']['cmodels']['#default_value'] = variable_get('delftdora_content_models', array());
  $form['islandora_transform_submit'] = array(
    '#value' => t('Save Configuration'),
    '#type' => 'submit',
  );
  return $form;
}

/**
 * Admin form submit.
 */
function delft_admin_submit($form, &$form_state) {
  $selected_cmodels = array_filter($form_state['values']['cmodels']);

  variable_set('delftdora_fedora_password', $form_state['values']['delftdora_fedora_password']);
  variable_set('delftdora_fedora_path', $form_state['values']['delftdora_fedora_path']);
  variable_set('delftdora_fedora_log_path', $form_state['values']['delftdora_fedora_log_path']);
  variable_set('delftdora_content_models', $selected_cmodels);

  drupal_set_message(t('Settings saved'));
}
