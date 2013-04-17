<?php

/**
 * @file
 *
 * Admin callbacks for Islandora Preingest Transform
 */

/**
 * Admin form.
 */
function delft_admin($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $form = array();
 $default_password = variable_get('delftdora_fedora_password', 'unset');
  $form['delftdora_fedora_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Password'),
    '#description' => t('Password for user fedoraAdmin.'),
    '#default_value' => $default_password,
  );

  $form['delftdora_fedora_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Fedora Executable Path'),
    '#description' => t('Full server Path to fedora-modify script'),
    '#default_value' => '/usr/local/fedora/server/bin',
  );

  $rels_xslt = variable_get('delft_rels_transform_xslt', FALSE);
  $rels_message = $rels_xslt ? 'Change RELS-EXT XSLT' : 'Add RELS-EXT XSLT';
  $rels_description = $rels_xslt ? t('Change xslt used for rels transformations.') : t('Add xslt to use for rels transformations.');

  if ($rels_xslt) {
    $form['rels_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Current RELS-EXT XSLT'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['rels_wrapper']['current'] = array(
      '#type' => 'item',
      '#markup' => "<pre>" . htmlentities("$rels_xslt") . "</pre>",
    );
  }

  $form['rels_xslt'] = array(
    '#type' => 'file',
    '#title' => t($rels_message),
    '#description' => $rels_description,
  );

  $form['table_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Content Models'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['table_wrapper']['the_table'] = islandora_content_model_select_table_form_element('delft_content_models');
  $form['islandora_transform_submit'] = array(
    '#value' => t('Save Configuration'),
    '#type' => 'submit',
  );
  return $form;
}

/**
 * admin fomr validator
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function delft_admin_validate($form, &$form_state) {
  if (isset($form_state['values'])) {
    $validators = array('file_validate_extensions' => array('xsl', 'xslt'));
    $rels_file = file_save_upload('rels_xslt', $validators);
    if ($rels_file) {
      if ($rels_file = file_move($rels_file, 'public://')) {
        $form_state['storage']['rels_file'] = $rels_file;
      }
      else {
        form_set_error('file', t('Failed to write the uploaded file to the site\'s file folder.'));
      }
    }
  }
}

/**
 * admin form submit
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function delft_admin_submit($form, &$form_state) {

  if (isset($form_state['storage']['rels_file'])) {
    $file = $form_state['storage']['rels_file'];
    unset($form_state['storage']['rels_file']);
    variable_set("delftdora_rels_transform_xslt", file_get_contents($file->uri));
  }

  $enabled = array_filter($form_state['values']['the_table']);
  variable_set('delftdora_content_models', $enabled);
  variable_set('delftdora_fedora_password', $form_state['values']['delftdora_fedora_password']);
}

